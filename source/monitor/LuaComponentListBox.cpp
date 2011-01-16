#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaComponentListBox.h"
#include "NrpComponentListbox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_COMPONENTLISTBOX( "CLuaComponentListBox" );

BEGIN_LUNA_METHODS(CLuaComponentListBox)
	LUNA_ILUALISTBOX_HEADER( CLuaComponentListBox )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, AddItem )
	LUNA_AUTONAME_FUNCTION( CLuaComponentListBox, GetSelectedObject )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaComponentListBox)
END_LUNA_PROPERTIES

CLuaComponentListBox::CLuaComponentListBox(lua_State *L, bool ex) : ILuaListBox( L, CLASS_COMPONENTLISTBOX, ex )							//�����������
{}

int CLuaComponentListBox::AddItem( lua_State *L )	//��������� ����� � ������ �����������
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaComponentListBox::AddItem need 2 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	INrpObject* object = _GetLuaObject< INrpObject, ILuaObject >( L, 3, true );
	
	int ret = -1;
	IF_OBJECT_NOT_NULL_THEN	ret = _object->addItem( text.ToWide(), object, -1 );			

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
		int selected = _object->getSelected();
		if( selected >= 0 )
			selObject = _object->getSelectedObject();
	}

	lua_pushlightuserdata( L, selObject );
	return 1;
}

const char* CLuaComponentListBox::ClassName()
{
	return ( CLASS_COMPONENTLISTBOX );
}
}//namespace nrp