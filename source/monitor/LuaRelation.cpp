#include "StdAfx.h"
#include "LuaRelation.h"

namespace nrp
{

Luna< CLuaRelation >::RegType CLuaRelation::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaRelation ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaRelation, SetValue ),
	LUNA_AUTONAME_FUNCTION( CLuaRelation, GetValue ),
	{0,0}
};

CLuaRelation::CLuaRelation(lua_State *L) : ILuaObject(L, "CLuaRelation")
{}

int CLuaRelation::SetValue( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTab::SetFontFromSize need name, value parameter");

	const char* name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		if( lua_isnumber( L, 3 ) )
			object_->SetValue( name, static_cast< float >( lua_tonumber( L, 3 ) ) );
		else if( lua_isstring( L, 3 ) )
			object_->SetValue( name, std::string( lua_tostring( L, 3 ) ) );
		else
			assert( "unsupport type" );
	}

	return 1;
}

int CLuaRelation::GetValue( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTab::GetValue need name parameter");

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( object_->IsValueTypeA<std::string>( name ) )
			lua_pushstring( L, object_->GetValue<std::string>( name ).c_str() );
		else if( object_->IsValueTypeA< float >( name ) )
			lua_pushnumber( L, object_->GetValue<float>( name ) );
		else if( object_->IsValueTypeA< int >( name ) )
			lua_pushinteger( L, object_->GetValue<int>( name ) );
		else
			lua_pushinteger( L, -1 );
	}

	return 1;
}
}//namespace nrp