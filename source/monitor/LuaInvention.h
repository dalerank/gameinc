#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpInvention;
}

namespace nrp
{
CLASS_NAME CLASS_LUAINVENTION( "CLuaInvention" );

class CLuaInvention : public ILuaProject<nrp::CNrpInvention>
{
public:
	static Luna<CLuaInvention>::RegType methods[];				//������ �������

	CLuaInvention(lua_State *L);		
	int AddUser( lua_State* L );
	int Load( lua_State* L );
	int Remove( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int HaveRequireTech( lua_State* L );
	int GetFutureTechNumber( lua_State* L );
	int GetLevel( lua_State* L );
	int GetStatus( lua_State* L );
	int GetDescriptionLink( lua_State* L );
	int GetInvestiment( lua_State* L );
	int SetInvestiment( lua_State* L );
	int GetMonthPay( lua_State* L );
	int ClearMonthPay( lua_State* L );
	int GetSpeed( lua_State* L );
	int GetPrognoseDateFinish( lua_State* L );
	int GetPassedMoney( lua_State* L );
	int CheckParams( lua_State* L );
	int GetUserNumber( lua_State* L );
	int GetUser( lua_State* L );
	int GetDayLeft( lua_State* L );
	int IsValid( lua_State* L );
	int GetCompany( lua_State* L );
	int GetInternalName( lua_State* L );

	static const char* ClassName() { return CLASS_LUAINVENTION.c_str(); }	
	virtual std::string ObjectName() { return CLASS_LUAINVENTION; }
};

}//namespace nrp