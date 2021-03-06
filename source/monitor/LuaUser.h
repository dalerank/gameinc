#pragma once
#include "ILuaObject.h"

namespace nrp
{
	class CNrpUser;
}

namespace nrp
{

class CLuaUser : public ILuaObject<nrp::CNrpUser>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaUser)
	static const char* ClassName();

	CLuaUser(lua_State *L, bool );

	int SetSkill( lua_State* L );
	int GetSkill( lua_State* L );
	int SetCharacter( lua_State* L );
	int GetTypeName( lua_State* L );
	int IsFreeUser( lua_State* L );
	int GetParam( lua_State* L );
	int SetParam( lua_State* L );
	int AddParam( lua_State* L );
	int GetName( lua_State* L );
	int IsTypeAs( lua_State* L );
	int AddWork( lua_State* L );
	int GetWorkNumber( lua_State* L);
	int RemoveWork( lua_State* L );
	int HaveInvention( lua_State* L );
	int GetWork( lua_State* L );
	int Save( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int GetRelation( lua_State* L );
	int AddModificator( lua_State* L );
	int AddLearning( lua_State* L );
	int YetLearning( lua_State* L );
	int AddTimeWork( lua_State* L );
	int GetQuality( lua_State* L );
	int SetQualityAward( lua_State* L );
	int GetQualityAward( lua_State* L );
	int GetSalary( lua_State* L );
	int SetSalary( lua_State* L );
	int GetKnowledges( lua_State* L );
};

}//namespace nrp