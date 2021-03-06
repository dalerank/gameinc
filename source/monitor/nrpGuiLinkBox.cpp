#include "StdAfx.h"
#include "nrpGuiLinkBox.h"
#include "ImageGUISkin.h"
#include "INrpProject.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//! constructor
CNrpGuiLinkBox::CNrpGuiLinkBox( IGUIEnvironment* environment, 
						  IGUIElement* parent,
						  s32 id, 
						  const core::recti& rectangle, 
						  bool noclip)
			: CNrpButton( environment, parent, id, rectangle), _defaultImage( NULL ),
              _relTextAlign( RTA_CENTER ) 
{
	moduleType_ = nrp::PT_UNKNOWN;
	data_ = NULL;

    if( CImageGUISkin* skin = dynamic_cast< CImageGUISkin* >( environment->getSkin() ) )
    {
        setImage( skin->Config.GetConfig( this, SImageGUISkinConfig::LinkBox, SImageGUISkinConfig::Normal ).texture );
        setHoveredImage( skin->Config.GetConfig( this, SImageGUISkinConfig::LinkBox, SImageGUISkinConfig::Hovered ).texture );
        setPressedImage( skin->Config.GetConfig( this, SImageGUISkinConfig::LinkBox, SImageGUISkinConfig::Pressed ).texture );
        setScaleImage( true );
    }
}

gui::EGUI_ELEMENT_TYPE CNrpGuiLinkBox::getType()
{
	return EGUI_ELEMENT_TYPE(EGUIET_GUILINK);
}

//! destructor
CNrpGuiLinkBox::~CNrpGuiLinkBox()
{
}

//! called if an event happened.
bool CNrpGuiLinkBox::OnEvent(const SEvent& event)
{
	if (!IsEnabled)
		return Parent ? Parent->OnEvent(event) : false;

	switch(event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		if (event.KeyInput.PressedDown &&
			(event.KeyInput.Key == KEY_RETURN || 
			event.KeyInput.Key == KEY_SPACE))
		{
			if (!isPushButton_)
				setPressed(true);
			else
				setPressed(!pressed_);

			return true;
		}
		if (pressed_ && !isPushButton_ && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
		{
			setPressed(false);
			return true;
		}
		else
			if (!event.KeyInput.PressedDown && pressed_ &&
				(event.KeyInput.Key == KEY_RETURN || 
				event.KeyInput.Key == KEY_SPACE))
			{
				//Environment->removeFocus(this);

				if (!isPushButton_)
					setPressed(false);

				return true;
			}
			break;
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
		{
			if (event.GUIEvent.Caller == this && !isPushButton_)
				setPressed(false);
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch( event.MouseInput.Event  )
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
					return _ButtonLMouseDown( event );
		case EMIE_RMOUSE_LEFT_UP:
		case EMIE_LMOUSE_LEFT_UP:
					return _ButtonMouseUp( event );
		}
		break;
	default:
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}

bool CNrpGuiLinkBox::_ButtonMouseUp( const irr::SEvent& event )
{
	bool wasPressed = pressed_;

	if ( !AbsoluteClippingRect.isPointInside( core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y ) ) )
	{
		if (!isPushButton_)
			setPressed(false);
		return true;
	}

	if (!isPushButton_)
		setPressed(false);
	else
		setPressed(!pressed_);

	if ((!isPushButton_ && wasPressed && Parent) ||
		(isPushButton_ && wasPressed != pressed_))
	{
		CNrpGUIEnvironment* ge = dynamic_cast< CNrpGUIEnvironment* >( Environment );
		if( ge && ge->getDragObject() )
			PCall( GUIELEMENT_SET_DATA, this, ge->getDragObject() );

		if( isDraggable_ && ge)
		{
			ge->setDragObject( this, image_ );
			PCall( GUIELEMENT_DRAG_START, this, NULL );
		}
	}

	PCall( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP
		                  ? GUIELEMENT_LMOUSE_LEFTUP
						  : GUIELEMENT_RMOUSE_LEFTUP, this );
	return true;
}

bool CNrpGuiLinkBox::_ButtonLMouseDown( const irr::SEvent& event )
{
	if (Environment->hasFocus(this) &&
		!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
	{
		Environment->removeFocus(this);
		return false;
	}

	if (!isPushButton_)
		setPressed(true);

	Environment->setFocus(this);
	PCall( GUIELEMENT_LMOUSE_DOWN, this );
	return true;
}

//! draws the element and its children
void CNrpGuiLinkBox::draw()
{
	if (!IsVisible)
		return;

	video::IVideoDriver* driver = Environment->getVideoDriver();
	IGUIFont* font = overrideFont_ != NULL ? overrideFont_ : Environment->getSkin()->getFont(EGDF_BUTTON);
	core::rect<s32> rect = AbsoluteRect;

	gui::IGUIElement* elmDrag = dynamic_cast< gui::CNrpGUIEnvironment* >( Environment )->getDragObject();
	bool isHovered = false;

	if( elmDrag && Environment->isHovered( this ) && elmDrag->getType() == gui::EGUIET_GUILINK )
		isHovered = dynamic_cast< CNrpGuiLinkBox* >( elmDrag )->getModuleType() == moduleType_;

	// todo:	move sprite up and text down if the pressed state has a sprite
	//			draw sprites for focused and mouse-over 
	core::position2di spritePos = AbsoluteRect.getCenter();
	core::recti txsRect;
	video::ITexture* texImage = NULL;
	s32 btnState = 0;

	core::position2d<s32> pos = AbsoluteRect.getCenter();
	pos.X -= imageRect_.getWidth() / 2;
	pos.Y -= imageRect_.getHeight() / 2;

	if( pressed_ )
	{
		texImage = pressedImage_;
		txsRect = pressedImageRect_;
		btnState = EGBS_BUTTON_DOWN;
		if (image_ == pressedImage_ && pressedImageRect_ == imageRect_)
		{
			pos.X += 1;
			pos.Y += 1;
		}
	}
	else
	{
		texImage = image_;
		txsRect = imageRect_;
		btnState = EGBS_BUTTON_UP;
	}

	video::SColor color( AlphaBlend, AlphaBlend, AlphaBlend, AlphaBlend );
	video::SColor colors[4] = { color, color, color, color };	

	if( texImage != NULL )
	{
		driver->draw2DImage( texImage, AbsoluteRect, txsRect, &AbsoluteClippingRect, colors, true  );		
	}
	else if( border_ )
	{		
		if( _defaultImage )
			driver->draw2DImage( _defaultImage, AbsoluteRect, _defaultImageRect, &AbsoluteClippingRect, colors, true );
		else
		{
			core::recti imgRect = rect;
			imgRect.UpperLeftCorner += core::position2di( 5, 5 );
			imgRect.LowerRightCorner -= core::position2di( 5, 5 );

			driver->draw2DRectangle( video::SColor( isHovered ? 
					 				 0x800000ff :
									(data_ ? 0x8000ff00 : 0x80ff0000) ), imgRect, &AbsoluteClippingRect );
		}
	}

	if (Text.size())
	{
		rect = AbsoluteRect;
        bool hcenter=true, vcenter=true;
        switch( _relTextAlign )
        {
        case RTA_LEFT: 
                AbsoluteRect -= core::position2di( AbsoluteRect.getWidth(), 0 );                    
        break;
        case RTA_BOTTOM: 
                vcenter = false; 
                AbsoluteRect += core::position2di( 0, AbsoluteRect.getHeight() );
        break;
        case RTA_TOP: 
                AbsoluteRect -= core::position2di( 0, AbsoluteRect.getHeight() / 2 );
        break;

        case RTA_RIGHT:
                AbsoluteRect += core::position2di( AbsoluteRect.getWidth() );
        break;        
        }

		if (font)
			font->draw(Text.c_str(), rect,
			overrideColorEnabled_ ? overrideColor_ : Environment->getSkin()->getColor(IsEnabled ? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT), hcenter, vcenter, 
			0 );
	}

	IGUIElement::draw();
}

void CNrpGuiLinkBox::SetData( void* data )
{
	data_ = data;
}

void CNrpGuiLinkBox::setDefaultImage( video::ITexture* image )
{
	if (_defaultImage)
		_defaultImage->drop();

	_defaultImage = image;
	if (image)
		_defaultImageRect = core::rect<s32>(core::position2d<s32>(0,0), image->getOriginalSize());

	if (_defaultImage)
		_defaultImage->grab();}

void CNrpGuiLinkBox::setTextPos( REL_TEXT_ALIGN align )
{
       _relTextAlign = align;
}

}//namespace gui

}//namespace irr