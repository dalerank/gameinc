#include "StdAfx.h"
#include <IGUIButton.h>
#include <irrlicht.h>
#include <assert.h>

#include "LuaButton.h"
#include "nrpEngine.h"
#include "nrpButton.h"
#include "nrpGUIEnvironment.h"
#include "nrpButton.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUABUTTON( "CLuaButton" );

Luna< CLuaButton >::RegType CLuaButton::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaButton ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetHoveredImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetPressedImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetAction ),
	{0,0}
};

const char* CLuaButton::ClassName()
{
	return 	( CLASS_LUABUTTON );
}

CLuaButton::CLuaButton(lua_State *L)	: ILuaGuiElement(L, CLASS_LUABUTTON )							//�����������
{}

int CLuaButton::SetImage( lua_State *L )							//�������� ��� ����� � ���������, ������� �� ������� ���� ����� ������
																	//��� �������� ���������
{
	return SetImage_( L, "SetHoveredImage", TI_IMAGE );
}

int CLuaButton::SetHoveredImage( lua_State *L )						//�������� ��� ����� � ���������, ������� �� ������� ���� ����� ������
																	//��� �������� ���������
{
	return SetImage_( L, "SetHoveredImage", TI_HOVER );
}

int CLuaButton::SetPressedImage( lua_State *L )						//�������� ��� ����� � ���������, ������� �� ������� ���� ����� ������
																	//��� �������� ���������
{
	return SetImage_( L, "SetPressedImage", TI_PRESSED );
}
	
int CLuaButton::SetAction( lua_State *L )									//������������� ��� ����� ������� ��� ���� ������	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetAction need string parameter");

	const char* funcName = lua_tostring( L, 2 );
	assert( funcName != NULL );

	IF_OBJECT_NOT_NULL_THEN	dynamic_cast< gui::CNrpButton* >( object_ )->setOnClickAction( funcName );

	return 1;
}

int CLuaButton::SetImage_( lua_State* L, const NrpText& funcName, TYPE_IMAGE typeimg )
{
	int argc = lua_gettop(L);
	//�������� �� ������� ���������, ��� ������ ���� ������� � ���������	
	luaL_argcheck(L, argc == 6, 6, _ErrStr( funcName + " need string, rectangle parameter" ) ); 

	core::recti rectangle;
	const char* texturepath = lua_tostring( L, 6 );
	assert( texturepath != NULL );
	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath );

	rectangle = _ReadRect( L, 2 );
	if( rectangle == core::recti( 0, 0, 0, 0) && ( txs != NULL ) )
		rectangle = core::recti( 0, 0, txs->getSize().Width, txs->getSize().Height );

	IF_OBJECT_NOT_NULL_THEN
		switch( typeimg ) 
		{
		case TI_IMAGE: object_->setImage( txs, rectangle ); break;
		case TI_HOVER: dynamic_cast< gui::CNrpButton* >( object_ )->setHoveredImage( txs, rectangle ); break;
		case TI_PRESSED: object_->setPressedImage( txs, rectangle ); break;
		}

	return 1;
}
}//namespace nrp