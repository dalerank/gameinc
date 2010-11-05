#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaPictureFlow.h"
#include "nrpEngine.h"
#include "Nrp2DPictureFlow.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaPictureFlow >::RegType CLuaPictureFlow::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaPictureFlow ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetSelected ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, Clear ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelectedObject ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, GetSelectedItem ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetPictureRect ),
	LUNA_AUTONAME_FUNCTION( CLuaPictureFlow, SetDrawBorder ),
	{0,0}
};

CLuaPictureFlow::CLuaPictureFlow(lua_State *L)	: ILuaGuiElement(L, CLASS_LUAPICTUREFLOW )							//�����������
{}

int CLuaPictureFlow::AddItem( lua_State *L )	//��������� ����� � ������ �����������
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaPictureFlow::AddItem need 2 parameter");

	video::ITexture* texture = NULL;
	const char* texturePath = lua_tostring( L, 2 );
	if( texturePath )
		texture = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturePath );

	const char* text = lua_tostring( L, 3 ); 
	assert( text != NULL );
	void* object = lua_touserdata( L, 4 );
	
	IF_OBJECT_NOT_NULL_THEN	object_->addItem( texture, conv::ToWide( text ).c_str(), object );			

	return 1;
}

int CLuaPictureFlow::GetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelected not need any parameter");

	int selected = -1;

	IF_OBJECT_NOT_NULL_THEN selected = object_->getSelected();
	lua_pushinteger( L, selected );

	return 1;
}

int CLuaPictureFlow::SetSelected( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPictureFlow::SetSelected need int parameter");

	int selected = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setSelected( selected );			

	return 1;
}

int CLuaPictureFlow::Clear( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::Clear not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	object_->clear();			

	return 1;
}

int CLuaPictureFlow::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			selObject = (void*)object_->getObject( selected );
	}
	lua_pushlightuserdata( L, selObject );

	return 1;
}

int CLuaPictureFlow::SetPictureRect( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaPictureFlow::SetPictureRect need rectangle parameter");

	core::recti rectangle( lua_tointeger( L, 2 ), lua_tointeger( L, 3 ), lua_tointeger( L, 4 ), lua_tointeger( L, 5) );

	IF_OBJECT_NOT_NULL_THEN	object_->setPictureRect( rectangle );

	return 1;
}

int CLuaPictureFlow::GetSelectedItem( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPictureFlow::GetSelectedObject not need any parameter");

	std::string text = "";

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			text = conv::ToStr( object_->getListItem( selected ) );
	}

	lua_pushstring( L, text.c_str() );
	return 1;
}

int CLuaPictureFlow::SetDrawBorder( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPictureFlow::SetDrawBorder need int parameter");

	bool drawBorder = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->setDrawBackground( drawBorder );			

	return 1;
}

}//namespace nrp