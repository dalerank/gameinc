#include "StdAfx.h"
#include "LuaCompany.h"
#include "IUser.h"
#include "NrpCompany.h"
#include "NrpGameEngine.h"

namespace nrp
{

Luna< CLuaCompany >::RegType CLuaCompany::methods[] =			//���������� ������
{
	LUNA_ILUAOBJECT_HEADER( CLuaCompany ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaCompany, SetCEO ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetBalance ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetEnginesNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, AddGameEngine ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaCompany, GetTech ),
	{0,0}
};

CLuaCompany::CLuaCompany(lua_State *L)	: ILuaProject(L, "CLuaCompany")	//�����������
{}

int CLuaCompany::SetCEO( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:SetCEO need ptrUser parameter" );

	IUser* user = (IUser*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetOption<PUser>( nrp::CEO, user );

	return 1;
}

int CLuaCompany::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "" ).c_str() ); 
	return 1;
}

int CLuaCompany::GetBalance( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetBalance", BALANCE, 0 ) );
	return 1;
}

int CLuaCompany::GetEnginesNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetEnginesNumber", ENGINES_NUMBER, 0 ) );
	return 1;
}

int CLuaCompany::GetEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetEngine need int parameter" );

	int idx = lua_tointeger( L, 2 );
	CNrpGameEngine* eng = NULL;
	IF_OBJECT_NOT_NULL_THEN	eng = object_->GetGameEngine( idx );

	lua_pushlightuserdata( L, eng );
	return 1;
}

int CLuaCompany::AddGameEngine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:AddGameEngine need CNrpGameEngine parameter" );

	CNrpGameEngine* ptrGameEng = (CNrpGameEngine*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->AddGameEngine( ptrGameEng );

	return 1;
}

int CLuaCompany::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechNumber", TECHS_NUMBER, 0 ));
	return 1;
}

int CLuaCompany::GetTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCompany:GetTech need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN	tech = object_->GetTech( index );

	lua_pushlightuserdata( L, tech );
	return 1;
}
}//namespace nrp