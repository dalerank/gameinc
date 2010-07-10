#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaDevelopProject.h"
#include "INrpDevelopProject.h"
#include <assert.h>

namespace nrp
{

Luna< CLuaDevelopProject >::RegType CLuaDevelopProject::methods[] =			//���������� ������
{
	LUNA_ILUAPROJECT_HEADER( CLuaDevelopProject ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopProject, GetModule ),
	{0,0}
};

CLuaDevelopProject::CLuaDevelopProject(lua_State *L)	: ILuaProject(L, CLASS_LUADEVPROJECT)							//�����������
{}

int CLuaDevelopProject::GetModuleNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetModuleNumber", MODULE_NUMBER, 0) );
	return 1;
}

int CLuaDevelopProject::GetModule( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopProject:GetModule() need int parameter" );

	CNrpProjectModule* prj = NULL;
	int index = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN prj = object_->GetModule( index );

	lua_pushlightuserdata( L, prj );
	return 1;	
}
}//namespace nrp