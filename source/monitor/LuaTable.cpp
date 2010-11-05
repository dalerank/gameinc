#include "StdAfx.h"
#include "LuaTable.h"
#include "StrConversation.h"
#include "nrpEngine.h"
#include <irrlicht.h>

using namespace irr;

namespace nrp
{

Luna< CLuaTable >::RegType CLuaTable::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTable ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetAction ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, AddRow),
	LUNA_AUTONAME_FUNCTION( CLuaTable, AddColumn ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetColumnWidth ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetCellText ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, GetRowCount ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, GetColumnCount ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, RemoveColumn ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, SetRowHeight ),
	LUNA_AUTONAME_FUNCTION( CLuaTable, ClearRows ),
	{0,0}
};

CLuaTable::CLuaTable(lua_State *L) : ILuaGuiElement(L, "CLuaTable")						//�����������
{			
}

int CLuaTable::SetImage( lua_State *L )							//�������� ��� ����� � ���������, ������� �� ������� ���� ����� ������
	//��� �������� ���������
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaTable::setImage need 5 parameter");

	const char* texturepath = lua_tostring( L, 6 );
	assert( texturepath != NULL );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* txs = NULL;
		if( texturepath != NULL )
			txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath ); //������ �������� � ����������
		//object_->setImage( txs, rectangle );								//��������� �������� � ������			
	}

	return 1;
}

int CLuaTable::SetAction( lua_State *L )									//������������� ��� ����� ������� ��� ���� ������	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetAction need string parameter");

	const char* funcName = lua_tostring( L, 2 );
	assert( funcName != NULL );
	//dynamic_cast< gui::CNrpButton* >( table_ )->setOnClickAction( funcName.c_str() );

	return 1;
}

int CLuaTable::AddRow( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::AddRow need int parameter");

	int index = lua_tointeger( L, 2 );
	
	IF_OBJECT_NOT_NULL_THEN object_->addRow( index );

	return 1;	
}

int CLuaTable::AddColumn( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaButton::AddColumn need 2 parameter");

	const char* text = lua_tostring( L, 2 );
	assert( text != NULL );
	int index = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->addColumn( conv::ToWide( text ).c_str(), index );

	return 1;	
}

int CLuaTable::SetColumnWidth( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaButton::SetColumnWidth need 2 parameter");

	int index = lua_tointeger( L, 2 );
	int width = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->setColumnWidth( index, width );

	return 1;	
}

int CLuaTable::SetCellText( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 8, 8, "Function CLuaButton::SetCellText need 7 parameter");

	int aRow = lua_tointeger( L, 2 );
	int aCol = lua_tointeger( L, 3 );
	const char* text = lua_tostring( L, 4 );
	assert( text != NULL );
	int color = (lua_tointeger( L, 5 ) << 24);
	color += (lua_tointeger( L, 6 ) << 16);
	color += (lua_tointeger( L, 7 ) << 8);
	color += lua_tointeger( L, 8 );

	IF_OBJECT_NOT_NULL_THEN	object_->setCellText( aRow, aCol, conv::ToWide( text ).c_str(), video::SColor( color ) );

	return 1;
}

int CLuaTable::GetRowCount( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::GetRowCount not need parameter");

	int rowCount=-9999;

	IF_OBJECT_NOT_NULL_THEN  rowCount = object_->getRowCount();
	lua_pushinteger( L, rowCount );

	return 1;		
}

int CLuaTable::GetColumnCount( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::GetColumnCount not need parameter");

	int rowCount=-9999;

	IF_OBJECT_NOT_NULL_THEN  rowCount = object_->getColumnCount();
	lua_pushinteger( L, rowCount );

	return 1;	
}

int CLuaTable::RemoveColumn( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::RemoveColumn need int parameter");

	int pos=lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  object_->removeColumn( pos );
	
	return 1;	
}

int CLuaTable::SetRowHeight( lua_State *L )
{
	int argc = lua_gettop(L);luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetRowHeigth need 2 parameter");

	int height = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetItemHeight( height );
	return 1;	
}

int CLuaTable::ClearRows( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaButton::ClearRows not need parameter");

	IF_OBJECT_NOT_NULL_THEN  object_->clearRows();

	return 1;	
}
}//namespace nrp