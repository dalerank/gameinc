#pragma once

#include "ILuaObject.h"

namespace nrp
{

const std::string CLASS_LUAPLANT( "CLuaPlant" );
class CNrpPlant;

class CLuaPlant : public ILuaObject<CNrpPlant>
{
public:
	static Luna<CLuaPlant>::RegType methods[];				//������ �������

	CLuaPlant(lua_State *L);		
	int Load( lua_State* L );
	int LoadBaseReklame( lua_State* L );
	int LoadReklamePrice( lua_State* L );
	int SaveReklamePrice( lua_State* L );
	int AddReklameWork( lua_State* L );
	int Save( lua_State* L );
	int AddProduceWork( lua_State* L );
																//������� �� ��� ������
	static const char* StaticGetLuaName() { return CLASS_LUAPLANT.c_str(); }
};

}//namespace nrp