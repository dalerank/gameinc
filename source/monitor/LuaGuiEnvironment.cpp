#include "StdAfx.h"
#include <IVideoDriver.h>
#include <assert.h>

#include "LuaGuiEnvironment.h"
#include "nrpGUIEnvironment.h"
#include "NrpMiniMap.h"
#include "StrConversation.h"
#include "nrpGlobalMap.h"
#include "nrpChartCtrl.h"
#include "nrpGuiLinkBox.h"


using namespace irr;

namespace nrp
{

Luna< CLuaGuiEnvironment >::RegType CLuaGuiEnvironment::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaGuiEnvironment ),
	/*    */
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetRootGUIElement ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddWindow ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddBlenderAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddHoveredAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTextTimeAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddMiniMap ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddButton ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddScrollBar ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLuaAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetElementByID ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetElementByName ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddCircleScrollBar ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddRectAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTable ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddGlobalMap ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddChart ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddComboBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLabel ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddMoveAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddToDeletionQueue ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddEdit ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddImage ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddProgressBar ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTabControl ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddTab ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddLinkBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, AddCursorPosAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, RemoveAnimators ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetFocusedElement ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, SetDragObject ),
	LUNA_AUTONAME_FUNCTION( CLuaGuiEnvironment, GetDragObject ),
	{0,0}
};

CLuaGuiEnvironment::CLuaGuiEnvironment(lua_State *L) : ILuaObject(L, "CLuaGuiEnvironment")
{}

int CLuaGuiEnvironment::RemoveAnimators( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, ("Function " + ClassName() + ":RemoveAnimators not need any parameter").c_str() );

	gui::IGUIElement* elm = (gui::IGUIElement*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->RemoveAnimators( elm );

	return 1;    
}

int CLuaGuiEnvironment::AddTable( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function  CLuaGuiEnvironment:AddTable need 7 parameter");

	core::recti rectangle = ReadRect_( vm, 2 );

	s32 id = lua_tointeger( vm, 6 );
	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 7 );

	gui::IGUITable* table = NULL;
	IF_OBJECT_NOT_NULL_THEN table = object_->addTable( rectangle, parentElem, id, false );

	lua_pushlightuserdata( vm, (void*)table );

	return 1;
}

int CLuaGuiEnvironment::GetRootGUIElement( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 1, 1, "Function CLuaSceneManager:GetRootGUIElement not need any parameter");

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->getRootGUIElement();
	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddWindow( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddWindow need 7 parameter");

	const char* textureName = lua_tostring( vm, 2 );
	assert( textureName != NULL );
	core::recti rectangle = ReadRect_( vm, 3 );
	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 8 );

	gui::IGUIWindow* window = NULL;
	IF_OBJECT_NOT_NULL_THEN 
	{
		video::ITexture* txs = NULL;

	    txs = object_->getVideoDriver()->getTexture( textureName );

		window = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addWindow( txs, rectangle, 0, parentElem, id );
		window->drop();
	}
	 
	lua_pushlightuserdata( vm, (void*)window );

	return 1;
}

int CLuaGuiEnvironment::AddBlenderAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 5, 5, "Function CLuaGuiEnvironment:AddBlenderAnimator need 7 parameter");

	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 2 );
	s32 minb = lua_tointeger( vm, 3 );
	s32 maxb = lua_tointeger( vm, 4 );
	f32 stepb = (f32)lua_tonumber( vm, 5 );
	bool visibleOnEnd = lua_toboolean( vm, 6 ) > 0; 
	bool removeSelf = lua_toboolean( vm, 7 ) > 0;
	bool removeParent = lua_toboolean( vm, 8 ) > 0;

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addBlendAnimator( parentElem, minb, maxb, stepb,
																								   visibleOnEnd, removeSelf, removeParent );

	return 1;
}

int CLuaGuiEnvironment::AddHoveredAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddHoveredAnimator need 7 parameter");

	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 2 );
	u32 minb = lua_tointeger( vm, 3 );
	u32 maxb = lua_tointeger( vm, 4 );
	u32 stepb = lua_tointeger( vm, 5 );
	bool visibleOnEnd = lua_toboolean( vm, 6 ) > 0; 
	bool removeSelf = lua_toboolean( vm, 7 ) > 0;
	bool removeParent = lua_toboolean( vm, 8 ) > 0;

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addHoveredAnimator( parentElem, minb, maxb, stepb,
																									 visibleOnEnd, removeSelf, removeParent );

	return 1;
}


int CLuaGuiEnvironment::AddRectAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 11, 11, "Function CLuaGuiEnvironment:AddRectAnimator need 4 parameter");

	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 2 );
	core::recti minr, maxr;
	minr = ReadRect_( vm, 3 );
	maxr = ReadRect_( vm, 7 );
	s32 step = lua_tointeger( vm, 11 );

	gui::CNrpGUIEnvironment* env = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ );
	gui::IGUIAnimator* anim = NULL; 
	
	IF_OBJECT_NOT_NULL_THEN anim = env->addRectAnimator( parentElem, minr, maxr, step );
	
	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::AddLuaAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 3, 3, "Function CLuaGuiEnvironment:AddLuaAnimator need 2 parameter");

	gui::IGUIElement* parentElem = (gui::IGUIElement*)lua_touserdata( vm, 2 );
	const char* funcName = lua_tostring( vm, 3 );
	assert( funcName != NULL );

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addLuaAnimator( parentElem, funcName );

	return 1;
}

int CLuaGuiEnvironment::AddMiniMap( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddMiniMap need 6 parameter");

	core::recti rectangle = ReadRect_( vm, 2 );

	s32 id = lua_tointeger( vm, 6 );
	scene::ISceneNode* parentNode = (scene::ISceneNode*)lua_touserdata( vm, 7 );

	gui::CNrpMiniMap* mp = NULL;
	
	IF_OBJECT_NOT_NULL_THEN mp = new gui::CNrpMiniMap( rectangle, object_, id, parentNode );

	lua_pushlightuserdata( vm, (void*)mp );

	return 1;
}

int CLuaGuiEnvironment::AddButton( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddButton need 7 parameter");

	core::recti rectangle = ReadRect_( vm, 2 );

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 6 );
	s32 id = lua_tointeger( vm, 7 );
	const char* text = lua_tostring( vm, 8 );
	assert( text != NULL );

	gui::IGUIElement* elm = NULL;
	
	IF_OBJECT_NOT_NULL_THEN elm = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addButton( rectangle,  parent, id, StrToWide( text ).c_str() );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddScrollBar( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddScrollBar need 7 parameter");

	bool gorizontal = lua_toboolean( vm, 2 ) > 0;
	core::recti rectangle = ReadRect_( vm, 3 );

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 7 );
	s32 id = lua_tointeger( vm, 8 );

	gui::IGUIElement* elm = NULL;
	
	IF_OBJECT_NOT_NULL_THEN elm = object_->addScrollBar( gorizontal, rectangle,  parent, id );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::GetElementByID( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetElementByID need one parameter int ");

	int id = lua_tointeger(vm, 2);	

	gui::IGUIElement* sg = NULL;
	
	IF_OBJECT_NOT_NULL_THEN sg = object_->getRootGUIElement()->getElementFromId( id, true );
	lua_pushlightuserdata( vm, (void*)sg );

	return 1;
}

int CLuaGuiEnvironment::GetElementByName( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaSceneManager:GetElementByName need one parameter string ");

	const char* name = lua_tostring(vm, 2);	
	assert( name != NULL );

	gui::IGUIElement* sg = NULL;
	
	IF_OBJECT_NOT_NULL_THEN sg = object_->getRootGUIElement()->getChildenFromName( StrToWide( name ).c_str(), true );
	lua_pushlightuserdata( vm, (void*)sg );

	return 1;
}

int CLuaGuiEnvironment::AddCircleScrollBar( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaSceneManager:GetElementByID need 6 parameter ");

	core::recti rectangle = ReadRect_( vm, 2 );

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 6 );
	s32 id = lua_tointeger( vm, 7 );

	gui::IGUIScrollBar* scrb = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::CNrpGUIEnvironment* env = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ );
		scrb = env->addCicrcleScrollBar( parent, id, rectangle );
	}

	lua_pushlightuserdata( vm, (void*)scrb );

	return 1;
}

int CLuaGuiEnvironment::AddGlobalMap( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 6, 6, "Function CLuaSceneManager:GetElementByID need 5 parameter ");

	core::recti rectangle = ReadRect_( vm, 2 );

	s32 id = lua_tointeger( vm, 6 );

	gui::CNrpGloablMap* gb = NULL;
	
	IF_OBJECT_NOT_NULL_THEN gb = new gui::CNrpGloablMap( rectangle, object_, id );

	lua_pushlightuserdata( vm, (void*)gb );

	return 1;
}

int CLuaGuiEnvironment::AddChart( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaSceneManager:AddChart need 6 parameter ");

	core::recti rectangle = ReadRect_( vm, 2 );

	s32 id = lua_tointeger( vm, 6 );
	gui::IGUIElement* elm = (gui::IGUIElement*)lua_touserdata( vm, 7 );

	gui::CChartCtrl* chart = NULL;

	IF_OBJECT_NOT_NULL_THEN chart = new gui::CChartCtrl( object_, elm, id, rectangle );

	lua_pushlightuserdata( vm, (void*)chart );

	return 1;
}

int CLuaGuiEnvironment::AddComboBox( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddComboBox need 7 parameter");

	const char* texturepath = lua_tostring( vm, 2 );
	assert( texturepath != NULL );

	core::recti rectangle = ReadRect_( vm, 3 );

	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 8 );	

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->addComboBox( rectangle, parent, id );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddEdit( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddEdit need 7 parameter");

	const char* text = lua_tostring( vm, 2 );
	assert( text != NULL );

	core::recti rectangle = ReadRect_( vm, 3 );

	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 8 );	

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->addEditBox( StrToWide( text ).c_str(), rectangle, true, parent, id );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddLabel( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddLabel need 7 parameter");

	const char* text = lua_tostring( vm, 2 );
	assert( text != NULL );

	core::recti rectangle = ReadRect_( vm, 3 );

	s32 id = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 8 );	

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->addStaticText( StrToWide( text ).c_str(), rectangle, false, true, parent, id, false );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddMoveAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddMoveAnimator need 7 parameter");

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 2 );	
	
	core::position2di pos;
	pos.X = lua_tointeger( vm, 3 );
	pos.Y = lua_tointeger( vm, 4 );

	int step = lua_tointeger( vm, 5 );
	bool visibleOnStop = lua_toboolean( vm, 6 ) > 0;
	bool removeOnStop = lua_toboolean( vm, 7 ) > 0;
	bool removeParentOnStop =  lua_toboolean( vm, 8 ) > 0;

	gui::IGUIAnimator* anim = NULL;

	IF_OBJECT_NOT_NULL_THEN anim = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addMoveAnimator( parent, pos, step, 
																										 visibleOnStop,
																										 removeOnStop,
																										 removeParentOnStop );

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::AddToDeletionQueue( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaGuiEnvironment:AddToDeletionQueue need 2 parameter");

	gui::IGUIElement* elm = (gui::IGUIElement*)lua_touserdata( vm, 2 );	

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addToDeletionQueue( elm );

	return 1;
}

int CLuaGuiEnvironment::AddImage( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddImage need 7 parameter");

	core::recti rectangle = ReadRect_( vm, 2 );
	
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 6 );	

	s32 id = lua_tointeger( vm, 7 );
	const char* text = lua_tostring( vm, 8 );
	assert( text != NULL );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->addImage( rectangle, parent, id, StrToWide( text ).c_str() );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddTextTimeAnimator( lua_State *vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 2, 2, "Function CLuaGuiEnvironment:AddTextTimeAnimator need 1 parameter");

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 2 );	

	gui::IGUIAnimator* anim = NULL;

	IF_OBJECT_NOT_NULL_THEN anim = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addTextTimeAnimator( parent );

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::AddProgressBar( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddProgressBar need 6 parameter");

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 2 );	
	core::recti rectangle = ReadRect_( vm, 3 );
	s32 iid = lua_tointeger( vm, 7 );

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addProgressBar( parent, iid, rectangle );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddTabControl( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 7, 7, "Function CLuaGuiEnvironment:AddTabControl need 6 parameter");

	core::recti rectangle = ReadRect_( vm, 2 );
	s32 iid = lua_tointeger( vm, 6 );
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 7 );	
	
	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->addTabControl( rectangle, parent, true, true, iid );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddTab( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 4, 4, "Function CLuaGuiEnvironment:AddTab need 3 parameter");

	gui::IGUITabControl* parent = (gui::IGUITabControl*)lua_touserdata( vm, 2 );	
	const char* name = lua_tostring( vm, 3 );
	assert( name != NULL );
	int id = lua_tointeger( vm, 4 );

	gui::IGUITab* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = parent->addTab( StrToWide( name ).c_str(), id );

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddLinkBox( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 8, 8, "Function CLuaGuiEnvironment:AddLinkModule need 7 parameter");

	const char* name = lua_tostring( vm, 2 );
	assert( name != NULL );
	core::recti rectangle = ReadRect_( vm, 3 );
	s32 iid = lua_tointeger( vm, 7 );
	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 8 );	

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN
	{
		elm = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addLinkBox( parent, iid, rectangle );
		if( elm )
			elm->setText( StrToWide( name ).c_str() );
	}

	lua_pushlightuserdata( vm, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::AddCursorPosAnimator( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 4, 4, "Function CLuaGuiEnvironment:AddCursorPosAnimator need 3 parameter");

	gui::IGUIElement* parent = (gui::IGUIElement*)lua_touserdata( vm, 2 );
	core::position2di offset( lua_tointeger( vm, 3), lua_tointeger( vm, 4 ) );

	gui::IGUIAnimator* anim = NULL;

	IF_OBJECT_NOT_NULL_THEN anim = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->addCursorPosAnimator( parent, offset );

	lua_pushlightuserdata( vm, (void*)anim );

	return 1;
}

int CLuaGuiEnvironment::GetFocusedElement( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaSceneManager:GetFocusedElement not need any parameter");

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = object_->getFocus();
	lua_pushlightuserdata( L, (void*)elm );

	return 1;
}

int CLuaGuiEnvironment::SetDragObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGuiEnvironment:SetDragObject need ptrUser parameter" );

	gui::IGUIElement* elm = (gui::IGUIElement*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->setDragObject( elm );

	return 1;
}

int CLuaGuiEnvironment::GetDragObject( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaSceneManager:GetDragObject not need any parameter");

	gui::IGUIElement* elm = NULL;

	IF_OBJECT_NOT_NULL_THEN elm = dynamic_cast< gui::CNrpGUIEnvironment* >( object_ )->getDragObject();
	lua_pushlightuserdata( L, (void*)elm );

	return 1;
}
}//namespace nrp