#include "StdAfx.h"
#include "LuaApplication.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpPlayer.h"
#include "NrpAiUser.h"
#include "nrpEngine.h"
#include "NrpBank.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"

#include <assert.h>
#include <irrlicht.h>
using namespace irr;

namespace nrp
{

Luna< CLuaApplication >::RegType CLuaApplication::methods[] =			//���������� ������
{
	LUNA_ILUAOBJECT_HEADER( CLuaApplication ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateUser ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, UpdateGameTime ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetBank ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetPlayerCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, RemoveLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateGameProject ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, CreateTechnology ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, GetTech ),
	LUNA_AUTONAME_FUNCTION( CLuaApplication, AddPublicTechnology ),
	{0,0}
};

CLuaApplication::CLuaApplication(lua_State *L)	: ILuaProject(L, "CLuaApplication")	//�����������
{}

int CLuaApplication::CreateCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateCompany need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	CNrpCompany* cntr = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		cntr = new CNrpCompany( name );
		object_->AddCompany( cntr );
	}

	lua_pushlightuserdata( L, cntr );
	return 1;
}

int CLuaApplication::CreateUser( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaApplication:CreateUser need 2 parameter" );

	const char* userType = lua_tostring( L, 2 );
	assert( userType != NULL );

	const char* name = lua_tostring( L, 3 );
	assert( name != NULL );

	IUser* user = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		if( strcmp( userType, "player" ) == 0 )
		{
			user = new CNrpPlayer( name );
			object_->AddUser( true, user );
		}
		else if( strcmp( userType, "aiplayer" ) == 0 )
		{
			user = new CNrpAiUser( name );
			object_->AddUser( true, user );
		}
		else if( strcmp( userType, "coder") == 0 )
		{
			user = new IUser( "CLuaCoder", "" );
			object_->AddUser( false, user );
		}

		
	}
	lua_pushlightuserdata( L, (void*)user );

	return 1;
}

int CLuaApplication::UpdateGameTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:UpdateGameTime need 2 parameter" );

	bool dayChange=false, monthChange=false, yearChange=false;
	int idDateLabel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		bool needUpdate = object_->UpdateTime();

		wchar_t text[ 32 ] = { 0 };
		gui::IGUIEnvironment* env = CNrpEngine::Instance().GetGuiEnvironment();
		gui::IGUIElement* elm = env->getRootGUIElement()->getElementFromId( idDateLabel, true );
		SYSTEMTIME& time = object_->GetDateTime();
		if( elm && needUpdate )
		{
			swprintf_s( text, 31, L"%4d.%02d.%02d  %02d:%02d", time.wYear, time.wMonth, time.wDay, 
															   time.wHour, time.wMinute );
			elm->setText( text );
		}

		dayChange = lastDay_ != time.wDay;
		monthChange = lastMonth_ != time.wMonth;
		yearChange = lastYear_ != time.wYear;

		lastDay_ = time.wDay;
		lastMonth_ = time.wMonth;
		lastYear_ = time.wYear;
	}

	return 1;
}

int CLuaApplication::GetBank( lua_State* L )
{
	lua_pushlightuserdata( L, GetParam_<PNrpBank>(L, "GetBank", BANK, NULL ));
	return 1;
}

int CLuaApplication::GetPlayerCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaApplication:GetPlayerCompany not need parameter" );

	CNrpCompany* cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN  cmp = object_->GetPlayerCompany();

	lua_pushlightuserdata( L, (void*)cmp );
	return 1;
}

int CLuaApplication::AddRemLuaFunction_( lua_State* L, std::string funcName,  bool rem )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, ("Function CLuaApplication:" + funcName + " need string parameter").c_str() );

	int typea = lua_tointeger( L, 2 );
	const char* fName = lua_tostring( L, 3 );
	assert( fName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( rem ) object_->RemoveLuaFunction( typea, fName );
		else object_->AddLuaFunction( typea, fName );
	}

#ifdef _DEBUG
	char text[ MAX_PATH ];
	sprintf_s( text, MAX_PATH, "Object:%s %d  FuncName:%s\n", rem ? "remove" : "added", object_, fName );
	OutputDebugString( text );
#endif

	return 1;
}

int CLuaApplication::AddLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "AddLuaFunction", false );
}

int CLuaApplication::RemoveLuaFunction( lua_State* L )
{
	return AddRemLuaFunction_( L, "RemoveLuaFunction", true );
}

int CLuaApplication::CreateGameProject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateGameProject need string parameter" );

	const char* name = lua_tostring( L, 2 );
	INrpProject* eng = NULL;
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN eng = object_->CreateGameProject( name );
	lua_pushlightuserdata( L, eng );

	return 1;
}

int CLuaApplication::CreateGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateGameEngine need string parameter" );

	const char* name = lua_tostring( L, 2 );
	CNrpGameEngine* eng = NULL;
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN eng = object_->CreateGameEngine( name );
	lua_pushlightuserdata(L, eng );

	return 1;
}

int CLuaApplication::CreateTechnology( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateTechnology need int parameter" );

	int typen = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;

	IF_OBJECT_NOT_NULL_THEN tech = object_->CreateTechnology( typen );
	lua_pushlightuserdata(L, tech );

	return 1;
}

int CLuaApplication::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechNumber", TECHNUMBER, 0 ) );
	return 1;
}

int CLuaApplication::GetTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need int parameter" );

	int techNumber = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetTechnology( techNumber );

	lua_pushlightuserdata( L, tech );
	return 1;
}

int CLuaApplication::AddPublicTechnology( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need CNrpTechnology* parameter" );

	CNrpTechnology* tech = (CNrpTechnology*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->AddTechnology( tech );

	return 1;
}
}//namespace nrp