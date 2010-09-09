#include "StdAfx.h"
#include "LuaAutoScript.h"
#include "IUser.h"
#include "NrpInvention.h"
#include "NrpApplication.h"
#include "NrpCompany.h"

#include <assert.h>

namespace nrp
{

Luna< CLuaAutoScript >::RegType CLuaAutoScript::methods[] = 
{
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddUserToInvention ),
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddGameEngineToCompany ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	{0,0}
};

CLuaAutoScript::CLuaAutoScript(lua_State *L)
{}

int CLuaAutoScript::AddUserToInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaAutoScript:AddUserToInvention need userName, inventionName params" );

	const char* userName = lua_tostring( L, 2 );
	const char* inventionName = lua_tostring( L, 3 );
	assert( userName != NULL && inventionName != NULL );

	if( IUser* user = CNrpApplication::Instance().GetUser( userName ) )
		if( PNrpCompany cmp = user->GetValue<PNrpCompany>( PARENTCOMPANY ) )
			if( CNrpInvention* invention = cmp->GetInvention( inventionName ) )
			{
				invention->AddUser( user );
			}

	return 1;
}

int CLuaAutoScript::AddGameEngineToCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaAutoScript:AddGameEngineToCompany need engineName, companyName params" );

	const char* engineName = lua_tostring( L, 2 );
	const char* companyName = lua_tostring( L, 3 );
	assert( engineName != NULL && companyName != NULL );

	CNrpGameEngine* engine = CNrpApplication::Instance().GetGameEngine( engineName );
	PNrpCompany cmp = CNrpApplication::Instance().GetCompany( companyName );
	assert( cmp != NULL && engine != NULL );

	cmp->AddGameEngine( engine );

	return 1;	
}

}