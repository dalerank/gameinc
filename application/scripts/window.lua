local base = _G

module( "window" )

local function _Hide( sender )
	base.guienv:FadeAction( base.FADE_TIME, false, true )			
	sender = base.CLuaButton( sender )
	parent = base.CLuaWindow( sender.parent )

	base.guienv:AddTimer( base.AFADE_TIME, function() 
													  parent:Remove() 
													  base.guienv:FadeAction( base.FADE_TIME, true, true )			
										   end )	
end

function fsWindow( txs, action )
	base.guienv:FadeAction( base.FADE_TIME, false, false )	
	
	wnd = base.guienv:AddWindow( txs, 0, 0, "0e", "0e", -1, base.guienv.root )
	wnd.closeButton.visible = false
	wnd.draggable = false
	wnd.drawBody = false
	wnd.visible = false
	if action then wnd.onRemove = action end
	
	--adding closeButton
	base.button.Stretch( "50e", "50e", "0e", "0e", "button_down", wnd, -1, "",	_Hide )
	
	base.guienv:AddTimer( base.AFADE_TIME, function()
												wnd.visible = true
												base.guienv:FadeAction( base.FADE_TIME, true, true )
										   end )
	return wnd
end