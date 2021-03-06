#include "StdAfx.h"
#include "LuaQuestEngine.h"
#include "LuaQuest.h"
#include "OpFileSystem.h"

namespace nrp
{

CLASS_NAME CLASS_CLUAQUESTENGINE( "CLuaQuestEngine" );

BEGIN_LUNA_METHODS(CLuaQuestEngine)
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, GetActiveQuest )
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, GetQuest )
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, StartQuest )
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, ObsoleteQuest )
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, AddResourceDirectory )
	LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, AddActiveQuest )
    LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, SetResult )
    LUNA_AUTONAME_FUNCTION( CLuaQuestEngine, GetResult )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaQuestEngine)
	LUNA_AUTONAME_PROPERTY( CLuaQuestEngine, "numberActives", GetNumAQuests, PureFunction )
END_LUNA_PROPERTIES
	
CLuaQuestEngine::CLuaQuestEngine(lua_State *L, bool ex) : ILuaProject(L, CLASS_CLUAQUESTENGINE, ex )
{
}

CLuaQuestEngine::~CLuaQuestEngine(void)
{
}

const char* CLuaQuestEngine::ClassName()
{
	return CLASS_CLUAQUESTENGINE;
}

int CLuaQuestEngine::GetActiveQuest( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:GetActiveQuest need index or quest name as parameter" );

	IF_OBJECT_NOT_NULL_THEN 
	{
		CNrpQuest* quest = NULL;
		if( lua_isnumber( L, 2 ) )
		{
			int index = lua_tointeger( L, 2 );
			quest = _object->GetActiveQuest( index )	;
		}
		else if( lua_isstring( L, 2 ) )
		{
			NrpText name = lua_tostring( L, 2 );
			quest = _object->GetActiveQuest( name );
		}
		else 
		{
			assert( "unsupported type param for CLuaQuestEngine:GetActiveQuest " );
			Log( HW ) << "unsupported type param #2 " << lua_typename( L, 2 ) << " for CLuaQuestEngine:GetActiveQuest " << term;
		}


		if( quest )
		{
			lua_pushlightuserdata( L, quest );
			Luna< CLuaQuest >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaQuestEngine::GetNumAQuests( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (*_object)[ ACTIVE_QUESTS_NUMBER ] );
		return 1;
	}
	
	lua_pushnil( L );
	return 1;
}

int CLuaQuestEngine::StartQuest( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:StartQuest need quest name as parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->StartQuest( name );

	return 0;
}

int CLuaQuestEngine::AddActiveQuest( lua_State* L )
{
	int argc = lua_gettop( L );
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:AddActiveQuest need quest name as parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->AddActiveQuest( name );

	return 0;
}

int CLuaQuestEngine::ObsoleteQuest( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:EndQuest need quest name as parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->ObsoleteQuest( name );

	return 0;
}

int CLuaQuestEngine::AddResourceDirectory( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:AddResourceDirectory need string parameter" );

	NrpText pathTo = lua_tostring( L, 2 );

	assert( OpFileSystem::IsExist( pathTo ) );
	CNrpQuestEngine::Instance().AddResourceDirectory( OpFileSystem::RemoveEndSlash( pathTo ) );
	return 1;
}

int CLuaQuestEngine::GetQuest( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaQuestEngine:GetQuest need quest name as parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{	
		CNrpQuest* qst = _object->GetQuest( name );

		if( qst )
		{
			lua_pushlightuserdata( L, qst );
			Luna< CLuaQuest >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaQuestEngine::SetResult( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 4, 4, "Function CLuaQuestEngine:SetResult need quest name, param, string as parameter" );

    NrpText quest = lua_tostring( L, 2 );
    NrpText paramName = lua_tostring( L, 3 );
    NrpText text = lua_tostring( L, 4 );

    IF_OBJECT_NOT_NULL_THEN _object->SetResult( quest, paramName, text );

    return 0;
}

int CLuaQuestEngine::GetResult( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 3, 3, "Function CLuaQuestEngine:GetResult need quest name, param as parameter" );

    NrpText quest = lua_tostring( L, 2 );
    NrpText paramName = lua_tostring( L, 3 );

    IF_OBJECT_NOT_NULL_THEN
    {
        NrpText ret = _object->GetResult( quest, paramName );

        if( ret != CNrpQuestEngine::UnExist )
        {
            lua_pushstring( L, ret );
            return 1;
        }
    }

    lua_pushnil( L );
    return 1;
}

}