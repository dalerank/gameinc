#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIStaticText;
	}
}

namespace nrp
{

class CLuaLabel : public ILuaGuiElement<irr::gui::IGUIStaticText>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaLabel)

	static const char* ClassName();
	CLuaLabel(lua_State *L, bool);		
																//������� �� ��� ������
	int SetOverrideColor( lua_State* L );
	int SetTextAlignment( lua_State* L );
	int SetFont( lua_State* L );
	int SetDrawBackground( lua_State* L );
	int SetWordWrap( lua_State* L );
	int SetImage( lua_State* L );
};

}//namespace nrp