#pragma once
#include <iguiwindow.h>
#include <vector>
#include "LuaFunctionality.h"

namespace nrp
{
	class CNrpUser;
}

namespace irr
{
namespace gui
{
class IGUIButton;

class CNrpUserInfoWindow : public IGUIWindow, public ILuaFunctionality
{
public:
	
	//! constructor
	CNrpUserInfoWindow( IGUIEnvironment* environment, 
						IGUIElement* parent, 
						video::ITexture* texture, 
						s32 id, 
						nrp::CNrpUser* ptrUser,
						core::rect<s32> rectangle);

	//! destructor
	virtual ~CNrpUserInfoWindow();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! update absolute position
	virtual void updateAbsolutePosition();

	//! draws the element and its children
	virtual void draw();

	//! Returns pointer to the close button
	virtual IGUIButton* getCloseButton() const;

	//! Returns pointer to the minimize button
	virtual IGUIButton* getMinimizeButton() const;

	//! Returns pointer to the maximize button
	virtual IGUIButton* getMaximizeButton() const;

	//! Returns true if the window is draggable, false if not
	virtual bool isDraggable() const;

	//! Sets whether the window is draggable
	virtual void setDraggable(bool draggable);

	//! Set if the window background will be drawn
	virtual void setDrawBackground(bool draw);

	//! Get if the window background will be drawn
	virtual bool getDrawBackground() const;

	//! Set if the window titlebar will be drawn
	//! Note: If the background is not drawn, then the titlebar is automatically also not drawn
	virtual void setDrawTitlebar(bool draw);

	//! Get if the window titlebar will be drawn
	virtual bool getDrawTitlebar() const;

	virtual void setBackgroundTexture( video::ITexture* texture );

	//! Writes attributes of the element.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

	//! Reads attributes of the element
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

	virtual const c8* getTypeName() const { return "CNrpWindow"; }

	virtual core::recti getClientRect() const;

	virtual void setAlphaBlend( u32 new_alpha );

protected:
	
	typedef enum { BTNE_CLOSE=0, BTNE_MINIMAZE, BTNE_MAXIMAZE } BUTTON_NAME;
	std::vector< IGUIButton* > buttons_;
	
	video::ITexture* background_;
	core::position2di DragStart;
	bool Dragging, IsDraggable;
	bool DrawBackground;
	bool DrawTitlebar;
	nrp::CNrpUser* user_;
};

} // end namespace gui
} // end namespace irr
