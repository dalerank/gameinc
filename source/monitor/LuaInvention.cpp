#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaInvention.h"
#include "NrpInvention.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "LuaUser.h"
#include "LuaCompany.h"
#include "LuaTechnology.h"

namespace nrp
{
CLASS_NAME CLASS_LUAINVENTION( "CLuaInvention" );

BEGIN_LUNA_METHODS(CLuaInvention)
	LUNA_AUTONAME_FUNCTION( CLuaInvention, AddUser )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Load )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, Create )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetPrognoseDateFinish )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, CheckParams )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, GetUser )
	LUNA_AUTONAME_FUNCTION( CLuaInvention, ClearMonthPay )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaInvention)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaInvention )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "level", GetLevel, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "status", GetStatus, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "description", GetDescriptionLink, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "investiment", GetInvestiment, SetInvestiment )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "speed", GetSpeed, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "userNumber", GetUserNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "passedMoney", GetPassedMoney, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "dayLeft", GetDayLeft, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "valid", IsValid, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "company", GetCompany, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaInvention, "monthPay", GetMonthPay, PureFunction )
END_LUNA_PROPERTIES

CLuaInvention::CLuaInvention(lua_State *L, bool ex) : ILuaProject( L, CLASS_LUAINVENTION, ex )							//конструктор
{}

int CLuaInvention::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaInvention:Create need int, company parameter" );

	CNrpTechnology* tech = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );
	PNrpCompany cmp = _GetLuaObject< CNrpCompany, CLuaCompany >( L, 3, false );

	assert( tech && cmp && "CLuaInvetntion must exist tech and company" );
	if( tech && cmp )
	{
		_object = new CNrpInvention( *tech, *cmp, _nrpApp[ CURRENTTIME ].As<NrpTime>() );
		lua_pushlightuserdata(L, _object );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaInvention::AddUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::AddUser not need parameter");

	CNrpUser* user = _GetLuaObject< CNrpUser, CLuaUser >( L, 2, false ); 
	assert( user );
	IF_OBJECT_NOT_NULL_THEN	
	{
		if( user )
			_object->AddUser( *user );
	}

	return 1;	
}

int CLuaInvention::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete _object;
		_object = NULL;
	}

	return 1;	
}

int CLuaInvention::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::Load not need parameter");

	NrpText iniFile = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_object->Load( iniFile );
	return 1;	
}

int CLuaInvention::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<TECH_STATUS>( L, PROP, STATUS, TS_UNKNOWN) );
	return 1;
}

int CLuaInvention::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, DESCRIPTIONPATH, "" ) );
	return 1;
}

int CLuaInvention::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, LEVEL, 0 ) );
	return 1;
}

int CLuaInvention::GetInvestiment( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, INVESTIMENT, 0 ) );
	return 1;
}

int CLuaInvention::GetSpeed( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, INVENTIONSPEED, 0 ) );
	return 1;
}

int CLuaInvention::GetPrognoseDateFinish( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		NrpTime time = (*_object)[ PROGNOSEDATEFINISH ];

		lua_pushinteger( L, time.RDay() );
		lua_pushinteger( L, time.RMonth() );
		lua_pushinteger( L, time.RYear() );
		return 3;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaInvention::SetInvestiment( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, PROP, INVESTIMENT, lua_tointeger );
}

int CLuaInvention::CheckParams( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:CheckParams not need parameter");

	IF_OBJECT_NOT_NULL_THEN	_object->CheckParams( _nrpApp[ CURRENTTIME ].As<NrpTime>() );

	return 1;
}

int CLuaInvention::GetUserNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, USERNUMBER, 0 ) );
	return 1;
}

int CLuaInvention::GetUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaInvention::Load not GetUser parameter");

	int index = lua_tointeger( L, 2 );
	CNrpUser* user = NULL;

	IF_OBJECT_NOT_NULL_THEN	user = _object->GetUser( index );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, user );
	Luna< CLuaUser >::constructor( L );

	return 1;	
}

int CLuaInvention::GetPassedMoney( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, PASSEDPRICE, 0 ) );
	return 1;
}

int CLuaInvention::GetDayLeft( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, DAYLEFT, 0 ) );
	return 1;
}

int CLuaInvention::IsValid( lua_State* L )
{
	bool avaible = true;
	try
	{
		IF_OBJECT_NOT_NULL_THEN	_object->ClassName();
	}
	catch(...)
	{
		avaible = false;
	}

	lua_pushboolean( L, avaible );
	return 1;
}

int CLuaInvention::GetCompany( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpCompany cmp = (*_object)[ PARENTCOMPANY ].As<PNrpCompany>();
		//lua_pop( L, lua_gettop( L ) );
		lua_pushlightuserdata( L, cmp );
		Luna< CLuaCompany >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaInvention::GetInternalName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetInternalName", INTERNAL_NAME, "" ) );
	return 1;
}

int CLuaInvention::GetMonthPay( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, MONEY_TODECREASE, 0 ) );
	return 1;
}

int CLuaInvention::ClearMonthPay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaInvention:ClearMonthPay not need parameter");

	IF_OBJECT_NOT_NULL_THEN (*_object)[ MONEY_TODECREASE ] = (int)0;

	return 1;
}

const char* CLuaInvention::ClassName()
{
	return ( CLASS_LUAINVENTION );
}
}//namespace nrp