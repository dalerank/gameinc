#include "StdAfx.h"
#include "NrpGuiLigthing.h"
#include "nrpGUIEnvironment.h"

#include <assert.h>

namespace irr
{

using namespace video;

namespace gui
{

// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//! constructor
CNrpGuiLigthing::CNrpGuiLigthing( IGUIEnvironment* guienv, 
								  gui::IGUIElement* firstElement,
								  gui::IGUIElement* secondElement,
								  s32 id, 
								  video::ITexture* texture,
								  s32 showTime )
								  : IGUIElement( EGUI_ELEMENT_TYPE( 0 ), guienv, guienv->getRootGUIElement(), id, core::recti( 0, 0, 0, 0) ), 
								  image_( texture ),
								  _elm1( firstElement ), _elm2( secondElement ), _showTime( showTime )
{
	_lastTimeUpdate = 0;
	_length = 40;
	_startTimeShow = GetTickCount();
	_imageRect = image_ ?  core::recti( core::position2di( 0, 0 ), image_->getSize() ) : core::recti( 0, 0, 20, 20 );
	_CreateNewPositions();
}

//! destructor
CNrpGuiLigthing::~CNrpGuiLigthing()
{
	if (image_)
		image_->drop();
}

void CNrpGuiLigthing::_CreateNewPositions()
{
	core::position2di pos1 = _elm1 ? _elm1->getAbsolutePosition().getCenter() : Environment->getCursorControl()->getPosition();
	core::position2di pos2 = _elm2 ? _elm2->getAbsolutePosition().getCenter() : Environment->getCursorControl()->getPosition();

	_positions.clear();
	_positions.push_back( pos1 ); 

	int radius = pos2.getDistanceFrom( pos1 ) / 4;
	if( radius < _length )
		return;

	int step = (radius * 4) / _length;

	core::line2df aLine( static_cast< float >( pos1.X ),
						 static_cast< float >( pos1.Y ), 
						 static_cast< float >( pos2.X ),
						 static_cast< float >( pos2.Y ) );

	if( image_ )
	{
		_length = aLine.getLength() / 12 + 1;
	}

	core::vector2df aVector = aLine.getUnitVector();
	for( s32 k=_positions.size(); k < _length; k++ )
	{
		core::vector2df tmpVector = aVector * static_cast< float >( k * step );
		core::position2di tt( tmpVector.X + rand() % 5, tmpVector.Y + rand() % 10 );
		_positions.push_back( tt + pos1 );
	}
}

//! draws the element and its children
void CNrpGuiLigthing::draw()
{
	if (!IsVisible)
		return;

	video::IVideoDriver* driver = Environment->getVideoDriver();

	for( size_t k=1; k < _positions.size()-1; k++ )
	{
		video::SColor colors[] = { _opacity, _opacity, _opacity, _opacity };
		driver->draw2DImage( image_, core::recti( _positions[ k ], core::dimension2du( 12, 12 ) ),
							 _imageRect, NULL, colors, true );
		//driver->draw2DLine( _positions[ k-1 ], _positions[ k ], video::SColor( 0xff000000 ) );
	}

	if( GetTickCount() - _lastTimeUpdate > 70 )
	{
		_CreateNewPositions();
		_lastTimeUpdate = GetTickCount();

		s32 diffTime = GetTickCount() - _startTimeShow;

		s32 rr = static_cast< s32 >( ( 1 - diffTime / static_cast< float >( _showTime * 1000 ) ) * 0xff );
		_opacity = video::SColor( rr, rr, rr, rr );

		if( diffTime > _showTime * 1000 )
		{
			if( CNrpGUIEnvironment* env = dynamic_cast< CNrpGUIEnvironment* >( Environment ) )
				env->addToDeletionQueue( this );				
		}
	}

	IGUIElement::draw();
}

//! Sets an image which should be displayed on the button when it is in normal state. 
void CNrpGuiLigthing::setImage(video::ITexture* image)
{
	if (image_)
		image_->drop();

	image_ = image;
	_imageRect = core::recti( 0, 0, 20, 20 );

	if (image_)
	{
		_imageRect = core::recti( core::position2di( 0, 0 ), image_->getSize() );
		image_->grab();
	}
}

}//namespace gui

}//namespace irr