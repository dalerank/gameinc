#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpGuiLinkBox;
	}
}

namespace nrp
{

class CLuaLinkBox : public ILuaGuiElement<irr::gui::CNrpGuiLinkBox>
{
public:
	static Luna<CLuaLinkBox>::RegType methods[];				//������ �������

	CLuaLinkBox(lua_State *L);		
																//������� �� ��� ������
	static const char* StaticGetLuaName() { return "CLuaLinkBox"; }
	int AddLuaFunction( lua_State* L );
	int RemoveLuaFunction( lua_State* L );
	int SetModuleType( lua_State* L );
	int GetModuleType( lua_State* L );
	int SetDraggable( lua_State* L );
	int IsDraggable( lua_State* L );
	int SetData( lua_State* L );
	int GetData( lua_State* L );
	int HaveData( lua_State* L );
private:
	typedef enum { TI_IMAGE=0, TI_HOVER, TI_PRESSED } TYPE_IMAGE;
	int AddRemLuaFunction_( lua_State* L, std::string funcName, bool add );

};

}//namespace nrp