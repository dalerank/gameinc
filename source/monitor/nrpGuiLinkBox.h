#pragma once

#include "nrpButton.h"
#include "LuaFunctionality.h"

namespace irr
{

namespace gui
{

const int EGUIET_GUILINK = EGUIET_COUNT + 10;

class CNrpGuiLinkBox : public CNrpButton
{
public:

    typedef enum { RTA_TOP=1, RTA_LEFT=2, RTA_BOTTOM=4, RTA_RIGHT=8, RTA_CENTER=0x10, RTA_COUNT } REL_TEXT_ALIGN;

	//! constructor
	CNrpGuiLinkBox( IGUIEnvironment* guienv, 
					gui::IGUIElement* parent,
					s32 id, 
					const core::recti& rectangle,
					bool noclip=false);

	//! destructor
	virtual ~CNrpGuiLinkBox();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! draws the element and its children
	virtual void draw();

	//! 
	virtual void SetData( void* data );
	//! 
	virtual void* GetData() const { return data_; }

	//! 
	virtual void SetDraggable( bool isDraggable )
	{
		isDraggable_ = isDraggable;
	}

	virtual bool IsDraggable() const { return isDraggable_; }

	virtual void setDefaultImage( video::ITexture* image );

	int getModuleType() const
	{
		return moduleType_;
	}

	void setModuleType( int typen )
	{
		moduleType_ = typen;
	}

    void setTextPos( REL_TEXT_ALIGN align );

	gui::EGUI_ELEMENT_TYPE getType();

	virtual const c8* getTypeName() const
	{
		return "CNrpGuiLinkBox";
	}
protected:
	u32 clickTime_;

	video::ITexture* _defaultImage;
	core::recti _defaultImageRect;

	void* data_;

	int moduleType_;
	bool isDraggable_;
    REL_TEXT_ALIGN _relTextAlign;

	bool _ButtonMouseUp( const irr::SEvent& event );
	bool _ButtonLMouseDown( const irr::SEvent& event );
};

}//namespace gui

}//namspace irr