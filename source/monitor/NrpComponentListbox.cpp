#include "StdAfx.h"
#include "NrpComponentListbox.h"
#include "nrpConfig.h"
#include <string>

using namespace nrp;

namespace irr
{
	
namespace gui
{

CNrpComponentListbox::CNrpComponentListbox( gui::IGUIEnvironment* env, IGUIElement* parent,
								 s32 id, core::rect<s32> rectangle )
								 : IGUIListBox( env, parent, id, rectangle),
								 Selected(-1), 
								 ItemHeight(0),
								 TotalItemHeight(0), 
								 ItemsIconWidth(0), 
								 Font(0), 
								 IconBank(0),
								 ScrollBar(0), 
								 Selecting(false), 
								 DrawBack(true),
								 MoveOverSelect(true), 
								 selectTime(0), 
								 AutoScroll(true),
								 KeyBuffer(), 
								 LastKeyTime(0), 
								 HighlightWhenNotFocused(true)
{
	IGUISkin* skin = Environment->getSkin();

	const s32 s = skin->getSize( EGDS_SCROLLBAR_SIZE );

	ScrollBar = Environment->addScrollBar( false, core::rect<s32>( RelativeRect.getWidth() - s, 0, 
		RelativeRect.getWidth(), RelativeRect.getHeight()),
		this );

	ScrollBar->setSubElement(true);
	ScrollBar->setTabStop(false);
	ScrollBar->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT);
	ScrollBar->setVisible(false);

	ScrollBar->setPos(0);
	ScrollBar->grab();
	ScrollBar->drop();

	setNotClipped(false);

	// this element can be tabbed to
	setTabStop(true);
	setTabOrder(-1);

	updateAbsolutePosition();
}

//! destructor
CNrpComponentListbox::~CNrpComponentListbox()
{
}

//! returns amount of list items
u32 CNrpComponentListbox::getItemCount() const
{
	return Items.size();
}

//! returns string of a list item. the may be a value from 0 to itemCount-1
const wchar_t* CNrpComponentListbox::getListItem(u32 id) const
{
	if (id>=Items.size())
		return 0;

	return Items[id].text.c_str();
}


//! Returns the icon of an item
s32 CNrpComponentListbox::getIcon(u32 id) const
{
	if (id>=Items.size())
		return -1;

	return Items[id].icon;
}

//////////////////////////////////////////////////////////////////////////
void* CNrpComponentListbox::getObject( u32 id ) const
{
	if( id >= Items.size() )
		return NULL;

	return Items[ id ].obj;
}

//! adds a list item, returns id of item
u32 CNrpComponentListbox::addItem(const wchar_t* text, void *ptrObject )
{
	return addItem(text, ptrObject, -1);
}
//////////////////////////////////////////////////////////////////////////

u32 CNrpComponentListbox::addItem(const wchar_t* text )
{
	return addItem(text, NULL, -1);
}

//! adds a list item, returns id of item
void CNrpComponentListbox::removeItem(u32 id)
{
	if (id >= Items.size())
		return;

	if ((u32)Selected==id)
	{
		Selected = -1;
	}
	else if ((u32)Selected > id)
	{
		Selected -= 1;
		selectTime = GetTickCount();
	}

	Items.erase(id);

	recalculateItemHeight();
}


//! clears the list
void CNrpComponentListbox::clear()
{
	Items.clear();
	ItemsIconWidth = 0;
	Selected = -1;

	if (ScrollBar)
		ScrollBar->setPos(0);

	recalculateItemHeight();
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::recalculateItemHeight()
{
	IGUISkin* skin = Environment->getSkin();

	if (Font != skin->getFont())
	{
		if (Font)
			Font->drop();

		Font = skin->getFont();
		ItemHeight = 0;

		if (Font)
		{
			ItemHeight = Font->getDimension(L"A").Height + 4;
			Font->grab();
		}
	}

	TotalItemHeight = ItemHeight * Items.size();
	ScrollBar->setMax( TotalItemHeight - AbsoluteRect.getHeight()  );
	ScrollBar->setSmallStep( ItemHeight );

	ScrollBar->setVisible( !( TotalItemHeight <= AbsoluteRect.getHeight() ));
}

//! returns id of selected item. returns -1 if no item is selected.
s32 CNrpComponentListbox::getSelected() const
{
	return Selected;
}


//! sets the selected item. Set this to -1 if no item should be selected
void CNrpComponentListbox::setSelected(s32 id)
{
	if ((u32)id>=Items.size())
		Selected = -1;
	else
		Selected = id;

	selectTime = GetTickCount();

	recalculateScrollPos();
}

void CNrpComponentListbox::setSelected( const wchar_t *item )
{

}

//! called if an event happened.
bool CNrpComponentListbox::OnEvent(const SEvent& event)
{
	switch(event.EventType)
	{
	case EET_KEY_INPUT_EVENT:
		if (event.KeyInput.PressedDown &&
			(event.KeyInput.Key == KEY_DOWN ||
			event.KeyInput.Key == KEY_UP   ||
			event.KeyInput.Key == KEY_HOME ||
			event.KeyInput.Key == KEY_END  ||
			event.KeyInput.Key == KEY_NEXT ||
			event.KeyInput.Key == KEY_PRIOR ) )
		{
			s32 oldSelected = Selected;
			switch (event.KeyInput.Key)
			{
			case KEY_DOWN:
				Selected += 1;
				break;
			case KEY_UP:
				Selected -= 1;
				break;
			case KEY_HOME:
				Selected = 0;
				break;
			case KEY_END:
				Selected = (s32)Items.size()-1;
				break;
			case KEY_NEXT:
				Selected += AbsoluteRect.getHeight() / ItemHeight;
				break;
			case KEY_PRIOR:
				Selected -= AbsoluteRect.getHeight() / ItemHeight;
				break;
			default:
				break;
			}
			if (Selected >= (s32)Items.size())
				Selected = Items.size() - 1;
			else
				if (Selected<0)
					Selected = 0;

			recalculateScrollPos();

			// post the news

			if (oldSelected != Selected && Parent && !Selecting && !MoveOverSelect)
			{
				SEvent e;
				e.EventType = EET_GUI_EVENT;
				e.GUIEvent.Caller = this;
				e.GUIEvent.Element = 0;
				e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
				Parent->OnEvent(e);
			}

			return true;
		}
		else
			if (!event.KeyInput.PressedDown && ( event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE ) )
			{
				if (Parent)
				{
					SEvent e;
					e.EventType = EET_GUI_EVENT;
					e.GUIEvent.Caller = this;
					e.GUIEvent.Element = 0;
					e.GUIEvent.EventType = EGET_LISTBOX_SELECTED_AGAIN;
					Parent->OnEvent(e);
				}
				return true;
			}
			else if (event.KeyInput.PressedDown && event.KeyInput.Char)
			{
				// change selection based on text as it is typed.
				u32 now = GetTickCount();

				if (now - LastKeyTime < 500)
				{
					// add to key buffer if it isn't a key repeat
					if (!(KeyBuffer.size() == 1 && KeyBuffer[0] == event.KeyInput.Char))
					{
						KeyBuffer += L" ";
						KeyBuffer[KeyBuffer.size()-1] = event.KeyInput.Char;
					}
				}
				else
				{
					KeyBuffer = L" ";
					KeyBuffer[0] = event.KeyInput.Char;
				}
				LastKeyTime = now;

				// find the selected item, starting at the current selection
				s32 start = Selected;
				// dont change selection if the key buffer matches the current item
				if (Selected > -1 && KeyBuffer.size() > 1)
				{
					if (Items[Selected].text.size() >= KeyBuffer.size() &&
						KeyBuffer.equals_ignore_case(Items[Selected].text.subString(0,KeyBuffer.size())))
						return true;
				}

				s32 current;
				for (current = start+1; current < (s32)Items.size(); ++current)
				{
					if (Items[current].text.size() >= KeyBuffer.size())
					{
						if (KeyBuffer.equals_ignore_case(Items[current].text.subString(0,KeyBuffer.size())))
						{
							if (Parent && Selected != current && !Selecting && !MoveOverSelect)
							{
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
								Parent->OnEvent(e);
							}
							setSelected(current);
							return true;
						}
					}
				}
				for (current = 0; current <= start; ++current)
				{
					if (Items[current].text.size() >= KeyBuffer.size())
					{
						if (KeyBuffer.equals_ignore_case(Items[current].text.subString(0,KeyBuffer.size())))
						{
							if (Parent && Selected != current && !Selecting && !MoveOverSelect)
							{
								Selected = current;
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_LISTBOX_CHANGED;
								Parent->OnEvent(e);
							}
							setSelected(current);
							return true;
						}
					}
				}

				return true;
			}
			break;

	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case gui::EGET_SCROLL_BAR_CHANGED:
			if (event.GUIEvent.Caller == ScrollBar)
			{
				ScrollBar->setPos(ScrollBar->getPos() / ItemHeight * ItemHeight );
				return true;
			}
			break;
		case gui::EGET_ELEMENT_FOCUS_LOST:
			{
				if (event.GUIEvent.Caller == this)
					Selecting = false;
			}
		default:
			break;
		}
		break;

	case EET_MOUSE_INPUT_EVENT:
		{
			core::position2d<s32> p(event.MouseInput.X, event.MouseInput.Y);

			switch(event.MouseInput.Event)
			{
			case EMIE_MOUSE_WHEEL:
				ScrollBar->setPos(ScrollBar->getPos() + (s32)event.MouseInput.Wheel* (-ItemHeight) );
				return true;

			case EMIE_LMOUSE_PRESSED_DOWN:
				{
					Selecting = true;
					return true;
				}

			case EMIE_LMOUSE_LEFT_UP:
				{
					Selecting = false;

					if (isPointInside(p))
						selectNew(event.MouseInput.Y);

					return true;
				}

			case EMIE_MOUSE_MOVED:
				if (Selecting || MoveOverSelect)
				{
					if (isPointInside(p))
					{
						selectNew(event.MouseInput.Y, true);
						return true;
					}
				}
			default:
				break;
			}
		}
		break;
	case EET_LOG_TEXT_EVENT:
	case EET_USER_EVENT:
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::selectNew(s32 ypos, bool onlyHover)
{
	u32 now = GetTickCount();
	s32 oldSelected = Selected;

	// find new selected item.
	int k =  ScrollBar->getPos();
	if (ItemHeight!=0) 
		Selected = ((ypos - AbsoluteRect.UpperLeftCorner.Y - 1) + ScrollBar->getPos()) / ItemHeight;

	if (Selected<0)
		Selected = 0;
	else
		if ((u32)Selected >= Items.size())
			Selected = Items.size() - 1;

	recalculateScrollPos();

	// post the news
	if (Parent && !onlyHover)
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.Element = 0;
		event.GUIEvent.EventType = (Selected == oldSelected && now < selectTime + 500) ? EGET_LISTBOX_SELECTED_AGAIN : EGET_LISTBOX_CHANGED;
		Parent->OnEvent(event);
	}
	selectTime = now;
}

//! Update the position and size of the listbox, and update the scrollbar
void CNrpComponentListbox::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();

	recalculateItemHeight();
}

//! draws the element and its children
void CNrpComponentListbox::draw()
{
	if (!IsVisible)
		return;

	try
	{
		recalculateItemHeight(); // if the font changed

		IGUISkin* skin = Environment->getSkin();

		core::rect<s32>* clipRect = 0;

		// draw background
		core::rect<s32> frameRect(AbsoluteRect);

		// draw items

		core::rect<s32> clientClip(AbsoluteRect);
		clientClip.UpperLeftCorner.Y += 1;
		clientClip.UpperLeftCorner.X += 1;
		if (ScrollBar->isVisible())
			clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);

		clientClip.LowerRightCorner.Y -= 1;
		clientClip.clipAgainst(AbsoluteClippingRect);

		skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true,
			DrawBack, frameRect, &clientClip);

		if (clipRect)
			clientClip.clipAgainst(*clipRect);

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.X += 1;
		if (ScrollBar->isVisible())
			frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);

		frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;
	 
		int scrollpos = ScrollBar->getPos();
		frameRect.UpperLeftCorner.Y -= scrollpos;
		frameRect.LowerRightCorner.Y -= scrollpos;

		bool hl = (HighlightWhenNotFocused || Environment->hasFocus(this) || Environment->hasFocus(ScrollBar));

		for (s32 i=0; i<(s32)Items.size(); ++i)
		{
			if ( (frameRect.LowerRightCorner.Y > AbsoluteRect.UpperLeftCorner.Y) &&
				(frameRect.UpperLeftCorner.Y + 2 < AbsoluteRect.LowerRightCorner.Y) )
			{
				INrpConfig* pObject = (INrpConfig*)Items[ i ].obj;

				u32 ucolor =  0xC0C0C0C0;
				if (i == Selected && hl)
					ucolor = 0xC0000000;

				skin->draw2DRectangle(this, video::SColor( ucolor ), frameRect, &clientClip);

				core::rect<s32> textRect = frameRect;
				textRect.UpperLeftCorner.X += 3;

				if (Font)
				{
					if (IconBank && (Items[i].icon > -1))
					{
						core::position2di iconPos = textRect.UpperLeftCorner;
						iconPos.Y += textRect.getHeight() / 2;
						iconPos.X += ItemsIconWidth/2;

						if ( i==Selected && hl )
						{
							IconBank->draw2DSprite( (u32)Items[i].icon, iconPos, &clientClip,
								hasItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) ? getItemOverrideColor(i, EGUI_LBC_ICON_HIGHLIGHT) : getItemDefaultColor(EGUI_LBC_ICON_HIGHLIGHT),
								selectTime, GetTickCount(), false, true);
						}
						else
						{
							IconBank->draw2DSprite( (u32)Items[i].icon, iconPos, &clientClip,
								hasItemOverrideColor(i, EGUI_LBC_ICON) ? getItemOverrideColor(i, EGUI_LBC_ICON) : getItemDefaultColor(EGUI_LBC_ICON),
								0 , (i==Selected) ? GetTickCount() : 0, false, true);
						}
					}

					textRect.UpperLeftCorner.X += ItemsIconWidth+3;

					EGUI_LISTBOX_COLOR itbn = ( i==Selected && hl ) ? EGUI_LBC_TEXT_HIGHLIGHT : EGUI_LBC_TEXT;
					video::SColor itbncolor = hasItemOverrideColor( i, EGUI_LBC_TEXT_HIGHLIGHT ) 
																	? getItemOverrideColor( i, itbn ) 
																	: getItemDefaultColor( itbn );
					if( Items[ i ].obj == NULL )
					{
						Font->draw( Items[i].text.c_str(), textRect, itbncolor, false, true, &clientClip);
					}
					else
					{			
						wchar_t tmpstr[ 128 ];

						textRect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
						textRect.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X + 80;

						int percent = pObject->GetOption<int>( READYWORKPERCENT );
						std::wstring name = StrToWide( pObject->GetOption<std::string>( NAME ) );
						
						swprintf( tmpstr, 127, L"%s  (Ready:%d)", name.c_str(), percent );
						Font->draw( tmpstr, textRect, itbncolor, true, true, &clientClip );
					}
					textRect.UpperLeftCorner.X -= ItemsIconWidth+3;
				}
			}

			frameRect.UpperLeftCorner.Y += ItemHeight;
			frameRect.LowerRightCorner.Y += ItemHeight;
		}
	}
	catch(...)
	{
		//ErrLog( gfx ) << all << "������ ��������� ������ �������" << term;
	}

	IGUIElement::draw();
}
//////////////////////////////////////////////////////////////////////////

u32 CNrpComponentListbox::addItem(const wchar_t* text, s32 icon)
{
	return addItem( text, NULL, icon );
}
//! adds an list item with an icon
u32 CNrpComponentListbox::addItem(const wchar_t* text, void* ptrObject, s32 icon)
{
	ListItem i;
	i.text = text;
	i.icon = icon;
	i.obj = ptrObject;

	Items.push_back(i);
	recalculateItemHeight();
	recalculateItemWidth(icon);

	return Items.size() - 1;
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::setSpriteBank(IGUISpriteBank* bank)
{
	if (IconBank)
		IconBank->drop();

	IconBank = bank;
	if (IconBank)
		IconBank->grab();
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::recalculateScrollPos()
{
	if (!AutoScroll)
		return;

	const s32 selPos = (Selected == -1 ? TotalItemHeight : Selected * ItemHeight) - ScrollBar->getPos();

	if (selPos < 0)
	{
		ScrollBar->setPos(ScrollBar->getPos() + selPos);
	}
	else
		if (selPos > AbsoluteRect.getHeight() - ItemHeight)
		{
			ScrollBar->setPos(ScrollBar->getPos() + selPos - AbsoluteRect.getHeight() + ItemHeight);
		}
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::setAutoScrollEnabled(bool scroll)
{
	AutoScroll = scroll;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpComponentListbox::isAutoScrollEnabled() const
{
	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return AutoScroll;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpComponentListbox::getSerializationLabels(EGUI_LISTBOX_COLOR colorType, core::stringc & useColorLabel, core::stringc & colorLabel) const
{
	switch ( colorType )
	{
	case EGUI_LBC_TEXT:
		useColorLabel = "UseColText";
		colorLabel = "ColText";
		break;
	case EGUI_LBC_TEXT_HIGHLIGHT:
		useColorLabel = "UseColTextHl";
		colorLabel = "ColTextHl";
		break;
	case EGUI_LBC_ICON:
		useColorLabel = "UseColIcon";
		colorLabel = "ColIcon";
		break;
	case EGUI_LBC_ICON_HIGHLIGHT:
		useColorLabel = "UseColIconHl";
		colorLabel = "ColIconHl";
		break;
	default:
		return false;
	}
	return true;
}

//! Writes attributes of the element.
void CNrpComponentListbox::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIListBox::serializeAttributes(out,options);

	// todo: out->addString	("IconBank",		IconBank->getName?);
	out->addBool    ("DrawBack",        DrawBack);
	out->addBool    ("MoveOverSelect",  MoveOverSelect);
	out->addBool    ("AutoScroll",      AutoScroll);

	out->addInt("ItemCount", Items.size());
	for (u32 i=0;i<Items.size(); ++i)
	{
		core::stringc label("text");
		label += i;
		out->addString(label.c_str(), Items[i].text.c_str() );

		for ( s32 c=0; c < (s32)EGUI_LBC_COUNT; ++c )
		{
			core::stringc useColorLabel, colorLabel;
			if ( !getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel) )
				return;
			label = useColorLabel; label += i;
			if ( Items[i].OverrideColors[c].Use )
			{
				out->addBool(label.c_str(), true );
				label = colorLabel; label += i;
				out->addColor(label.c_str(), Items[i].OverrideColors[c].Color);
			}
			else
			{
				out->addBool(label.c_str(), false );
			}
		}
	}
}

//! Reads attributes of the element
void CNrpComponentListbox::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	clear();

	DrawBack        = in->getAttributeAsBool("DrawBack");
	MoveOverSelect  = in->getAttributeAsBool("MoveOverSelect");
	AutoScroll      = in->getAttributeAsBool("AutoScroll");

	IGUIListBox::deserializeAttributes(in,options);

	const s32 count = in->getAttributeAsInt("ItemCount");
	for (s32 i=0; i<count; ++i)
	{
		core::stringc label("text");
		ListItem item;

		label += i;
		item.text = in->getAttributeAsStringW(label.c_str());

		addItem(item.text.c_str(), item.obj, item.icon);

		for ( u32 c=0; c < EGUI_LBC_COUNT; ++c )
		{
			core::stringc useColorLabel, colorLabel;
			if ( !getSerializationLabels((EGUI_LISTBOX_COLOR)c, useColorLabel, colorLabel) )
				return;
			label = useColorLabel; label += i;
			Items[i].OverrideColors[c].Use = in->getAttributeAsBool(label.c_str());
			if ( Items[i].OverrideColors[c].Use )
			{
				label = colorLabel; label += i;
				Items[i].OverrideColors[c].Color = in->getAttributeAsColor(label.c_str());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::recalculateItemWidth(s32 icon)
{
	if (IconBank && icon > -1 &&
		IconBank->getSprites().size() > (u32)icon &&
		IconBank->getSprites()[(u32)icon].Frames.size())
	{
		u32 rno = IconBank->getSprites()[(u32)icon].Frames[0].rectNumber;
		if (IconBank->getPositions().size() > rno)
		{
			const s32 w = IconBank->getPositions()[rno].getWidth();
			if (w > ItemsIconWidth)
				ItemsIconWidth = w;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CNrpComponentListbox::setItem(u32 index, const wchar_t* text, s32 icon)
{
	setItem( index, text, NULL, icon );
}

//////////////////////////////////////////////////////////////////////////
void CNrpComponentListbox::setItem(u32 index, const wchar_t* text, void* ptrObject, s32 icon)
{
	if ( index >= Items.size() )
		return;

	Items[ index ].text = text;
	Items[ index ].icon = icon;
	Items[ index ].obj = ptrObject;

	recalculateItemHeight();
	recalculateItemWidth(icon);
}

//////////////////////////////////////////////////////////////////////////
s32 CNrpComponentListbox::insertItem(u32 index, const wchar_t* text, s32 icon)
{
	return insertItem( index, text, NULL, icon );
}

//! Insert the item at the given index
//! Return the index on success or -1 on failure.
s32 CNrpComponentListbox::insertItem(u32 index, const wchar_t* text, void* ptrObject, s32 icon)
{
	ListItem i;
	i.text = text;
	i.icon = icon;
	i.obj = ptrObject;

	Items.insert(i, index);
	recalculateItemHeight();
	recalculateItemWidth(icon);

	return index;
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::swapItems(u32 index1, u32 index2)
{
	if ( index1 >= Items.size() || index2 >= Items.size() )
		return;

	ListItem dummmy = Items[index1];
	Items[index1] = Items[index2];
	Items[index2] = dummmy;
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::setItemOverrideColor(u32 index, const video::SColor &color)
{
	for ( u32 c=0; c < EGUI_LBC_COUNT; ++c )
	{
		Items[index].OverrideColors[c].Use = true;
		Items[index].OverrideColors[c].Color = color;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::setItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType, const video::SColor &color)
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return;

	Items[index].OverrideColors[colorType].Use = true;
	Items[index].OverrideColors[colorType].Color = color;
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::clearItemOverrideColor(u32 index)
{
	for (u32 c=0; c < (u32)EGUI_LBC_COUNT; ++c )
	{
		Items[index].OverrideColors[c].Use = false;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpComponentListbox::clearItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType)
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return;

	Items[index].OverrideColors[colorType].Use = false;
}
//////////////////////////////////////////////////////////////////////////

bool CNrpComponentListbox::hasItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const
{
	if ( index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return false;

	return Items[index].OverrideColors[colorType].Use;
}
//////////////////////////////////////////////////////////////////////////

video::SColor CNrpComponentListbox::getItemOverrideColor(u32 index, EGUI_LISTBOX_COLOR colorType) const
{
	if ( (u32)index >= Items.size() || colorType < 0 || colorType >= EGUI_LBC_COUNT )
		return video::SColor();

	return Items[index].OverrideColors[colorType].Color;
}
//////////////////////////////////////////////////////////////////////////

video::SColor CNrpComponentListbox::getItemDefaultColor(EGUI_LISTBOX_COLOR colorType) const
{
	IGUISkin* skin = Environment->getSkin();
	if ( !skin )
		return video::SColor();

	switch ( colorType )
	{
	case EGUI_LBC_TEXT:
		return skin->getColor(EGDC_BUTTON_TEXT);
	case EGUI_LBC_TEXT_HIGHLIGHT:
		return skin->getColor(EGDC_HIGH_LIGHT_TEXT);
	case EGUI_LBC_ICON:
		return skin->getColor(EGDC_ICON);
	case EGUI_LBC_ICON_HIGHLIGHT:
		return skin->getColor(EGDC_ICON_HIGH_LIGHT);
	default:
		return video::SColor();
	}
}
//////////////////////////////////////////////////////////////////////////

void* CNrpComponentListbox::getSelectedObject()
{
	return Items[ Selected ].obj;
}

void CNrpComponentListbox::setItemHeight( s32 height )
{

}

void CNrpComponentListbox::setDrawBackground( bool draw )
{

}

}

}