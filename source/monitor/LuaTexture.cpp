#include "StdAfx.h"
#include "LuaTexture.h"

#include <ITexture.h>

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUATEXTURE( "CLuaTexture" );

BEGIN_LUNA_METHODS(CLuaTexture)
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTexture, GetSize )
	LUNA_AUTONAME_FUNCTION( CLuaTexture, Drop )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTexture)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaTexture )
	LUNA_AUTONAME_PROPERTY( CLuaTexture, "width", GetWidth, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTexture, "height", GetHeight, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTexture, "path", GetName, PureFunction )
END_LUNA_PROPERTIES

CLuaTexture::CLuaTexture(lua_State *L, bool ex) : ILuaObject( L, CLASS_LUATEXTURE, ex )
{}

int CLuaTexture::GetSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture:GetSize not need any parameter");

	core::dimension2di size;
	IF_OBJECT_NOT_NULL_THEN size = _object->getSize();

	lua_pushinteger( L, size.Width );
	lua_pushinteger( L, size.Height );

	return 2;
}

int CLuaTexture::GetName( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushstring( L, _object->getName().getPath().c_str() );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTexture::GetWidth( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int width = _object->getSize().Width;
		lua_pushinteger( L, width );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTexture::GetHeight( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int height = _object->getSize().Height;
		lua_pushinteger( L, height );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaTexture::Drop( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture:Drop not need any parameter");

	int height = 0;

	IF_OBJECT_NOT_NULL_THEN _object->drop();

	return 1;
}

const char* CLuaTexture::ClassName()
{
	return ( CLASS_LUATEXTURE );
}

}
