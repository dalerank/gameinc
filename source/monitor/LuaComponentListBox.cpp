#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaComponentListBox.h"
#include "NrpComponentListbox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaComponentListBox >::RegType CLuaComponentListBox::methods[] =			//���������� ������
{
	LUNA_ILUALISTBOX_HEADER( CLuaComponentListBox ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, AddItem ),
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, GetSelectedObject ),
	{0,0}
};

CLuaComponentListBox::CLuaComponentListBox(lua_State *L) : ILuaListBox( L, CLASS_COMPONENTLISTBOX )							//�����������
{}

int CLuaComponentListBox::AddItem( lua_State *L )	//��������� ����� � ������ �����������
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaComponentListBox::AddItem need 2 parameter");

	const char* text = lua_tostring( L, 2 );
	assert( text != NULL );
	INrpObject* object = (INrpObject*)lua_touserdata( L, 3 );
	
	int ret = -1;
	IF_OBJECT_NOT_NULL_THEN	ret = object_->addItem( StrToWide( text ).c_str(), object, -1 );			

	lua_pushinteger( L, ret );
	return 1;
}

int CLuaComponentListBox::GetSelectedObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaComponentListBox::GetSelectedObject not need any parameter");

	void* selObject = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = object_->getSelected();
		if( selected >= 0 )
			selObject = object_->getSelectedObject();
	}
	lua_pushlightuserdata( L, selObject );

	return 1;
}

}//namespace nrp