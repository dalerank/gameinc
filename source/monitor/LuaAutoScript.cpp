#include "StdAfx.h"
#include "LuaAutoScript.h"
#include "IUser.h"
#include "NrpInvention.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "NrpProjectModule.h"
#include "INrpDevelopProject.h"
#include "NrpLaborMarket.h"
#include "NrpGameMarket.h"

#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUAAUTOSCRIPT( "CLuaAutoScript" );

BEGIN_LUNA_METHODS(CLuaAutoScript)
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddUserToInvention )
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddUserToGameProject )
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddGameEngineToCompany )
	/************************************************************************/
	/*                                                                      */
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaAutoScript)
END_LUNA_PROPERTIES

CLuaAutoScript::CLuaAutoScript(lua_State *L, bool ex )
{
	_isExisting = ex;
}

int CLuaAutoScript::AddUserToGameProject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaAutoScript:AddUserToGameProject need userName, devName, devModule params" );

	NrpText userName = lua_tostring( L, 2 );
	NrpText projectName = lua_tostring( L, 3 );
	NrpText moduleName = lua_tostring( L, 4 );
	assert( userName != NULL && projectName != NULL && moduleName != NULL );

	if( CNrpUser* user = _nrpLaborMarkt.GetUser( userName ) )
		if( PNrpCompany cmp = (*user)[ PARENTCOMPANY ].As<PNrpCompany>() )
			if( INrpDevelopProject* devProject = cmp->GetDevelopProject( projectName ) )
				if( CNrpProjectModule* module = devProject->GetModule( moduleName ) )
				{
					user->AddWork( *module );
				}

	return 1;
}

int CLuaAutoScript::AddUserToInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaAutoScript:AddUserToInvention need userName, inventionName params" );

	NrpText userName = lua_tostring( L, 2 );
	NrpText inventionName = lua_tostring( L, 3 );

	if( CNrpUser* user = _nrpLaborMarkt.GetUser( userName ) )
		if( PNrpCompany cmp = (*user)[ PARENTCOMPANY ].As<PNrpCompany>() )
			if( CNrpInvention* invention = cmp->GetInvention( inventionName ) )
			{
				invention->AddUser( *user );
			}

	return 1;
}

int CLuaAutoScript::AddGameEngineToCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaAutoScript:AddGameEngineToCompany need engineName, companyName params" );

	NrpText engineName = lua_tostring( L, 2 );
	NrpText companyName = lua_tostring( L, 3 );

    CNrpGameEngine* engine = CNrpGameMarket::Instance().GetGameEngine( engineName );
	PNrpCompany cmp = _nrpApp.GetCompany( companyName );
	assert( cmp != NULL && engine != NULL );

	cmp->AddGameEngine( engine );

	return 1;	
}

const char* CLuaAutoScript::ClassName()
{
	return ( CLASS_LUAAUTOSCRIPT );
}

}