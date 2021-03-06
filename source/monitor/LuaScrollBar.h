#pragma once
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpScrollBar;
	}
}

namespace nrp
{

class CLuaScrollBar : public ILuaGuiElement< irr::gui::CNrpScrollBar >
{
public:
	DEFINE_PROPERTIES_AND_METHODS( CLuaScrollBar )
	static const char* ClassName();

	CLuaScrollBar(lua_State *L, bool );
	int SetTexture( lua_State *L );
	int SetAction( lua_State *L );
	int SetMax( lua_State* L );
	int SetMin( lua_State* L );
	int GetUpButton( lua_State* L );
	int GetDownButton( lua_State* L );
	int SetSliderTexture( lua_State* L );
	int GetPos( lua_State* L );
};

}//namespace nrp