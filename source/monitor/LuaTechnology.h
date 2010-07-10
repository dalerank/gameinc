#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpTechnology;
}

namespace nrp
{

class CLuaTechnology : public ILuaProject<nrp::CNrpTechnology>
{
public:
	static Luna<CLuaTechnology>::RegType methods[];				//������ �������

	CLuaTechnology(lua_State *L);		
	int SetTechType( lua_State* L );
	int SetBaseCode( lua_State* L );
	int SetAddingEngineCode( lua_State* L );
	int SetEngineTechRequire( lua_State* L );
	int SetEmployerSkillRequire( lua_State* L );
	int SetQuality( lua_State* L );
	int GetLevel( lua_State* L );
	int GetOptionAsInt( lua_State* L );
	int GetTechGroup( lua_State* L );
	int HaveLider( lua_State* L );
	int Load( lua_State* L );
	int Remove( lua_State* L );
	int Create( lua_State* L );
	int IsLoaded( lua_State* L );
	int ValidTime( lua_State* L );
	int SetTexture( lua_State* L );
	int GetTexture( lua_State* L );
	int HaveRequireTech( lua_State* L );
	int GetFutureTechNumber( lua_State* L );
	int GetFutureTech( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaTech"; }	
};

}//namespace nrp