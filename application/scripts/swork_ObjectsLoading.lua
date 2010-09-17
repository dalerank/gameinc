cityWindow = nil
laborWindow = nil
receptionWindow = nil
directorCabinetWindow = nil
laborWindow	= nil

function ApplicationLoadLaborScene()
	guienv:FadeAction( 3000, false )
	
	if laborWindow then
		laborWindow:SetVisible( true )
	else
		laborWindow = guienv:AddWindow( "media/laboratory_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		laborWindow:SetDraggable( false )
		local closeBtn = laborWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end	
	
	local btnVideo = guienv:AddButton( 730, 43, 730 + 229, 43 + 215, laborWindow:Self(), -1, "")
	btnVideo:SetImage( 0, 0, 229, 215, "media/buttons/techMapVideo_normal.png" )
	btnVideo:SetHoveredImage( 0, 0, 229, 215, "media/buttons/techMapVideo_select.png" )	
	btnVideo:SetPressedImage( 0, 0, 229, 215, "media/buttons/techMapVideo_select.png" )	
	btnVideo:SetAction( "" )
	
	local btnVideo = guienv:AddButton( 0, 0, 0 + 168, 0 + 144, laborWindow:Self(), -1, "")
	btnVideo:SetImage( 0, 0, 168, 144, "media/buttons/techMapSound_normal.png" )
	btnVideo:SetHoveredImage( 0, 0, 168, 144, "media/buttons/techMapSound_select.png" )	
	btnVideo:SetPressedImage( 0, 0, 168, 144, "media/buttons/techMapSound_select.png" )	
	btnVideo:SetAction( "" )
end

function ApplicationLoadCityScene()
	if cityWindow then
		cityWindow:SetVisible( true )
	else
		cityWindow = guienv:AddWindow( "media/city_map.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		cityWindow:SetDraggable( false )
		local closeBtn = cityWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end
	
	local btnOffice = guienv:AddButton( 730, 43, 730 + 229, 43 + 215, cityWindow:Self(), -1, "")
	btnOffice:SetImage( 0, 0, 229, 215, "media/buttons/office_normal.tga" )
	btnOffice:SetHoveredImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetPressedImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetAction( "ApplicationLoadOfficeScene" )
	
	local btnMedia = guienv:AddButton( 417, 82, 417 + 216, 82 + 226, cityWindow:Self(), -1, "")
	btnMedia:SetImage( 0, 0, 216, 226, "media/buttons/media_normal.tga" )
	btnMedia:SetHoveredImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetPressedImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetAction( "ApplicationLoadMediaScene" )
	
	local btnLabor = guienv:AddButton( 811, 198, 811 + 179, 198 + 154, cityWindow:Self(), -1, "")
	btnLabor:SetImage( 0, 0, 179, 154, "media/buttons/labor_normal.tga" )
	btnLabor:SetHoveredImage( 0, 0, 179, 154, "media/buttons/labor_select.tga" )	
	btnLabor:SetPressedImage( 0, 0, 179, 154, "media/buttons/labor_select.tga" )	
	btnLabor:SetAction( "ApplicationLoadLaborScene" )
	
	local btnBank = guienv:AddButton( 323, 389, 323 + 237, 389 + 271, cityWindow:Self(), -1, "")
	btnBank:SetImage( 0, 0, 237, 271, "media/buttons/bank_normal.tga" )
	btnBank:SetHoveredImage( 0, 0, 237, 271, "media/buttons/bank_select.tga" )	
	btnBank:SetPressedImage( 0, 0, 237, 271, "media/buttons/bank_select.tga" )	
	btnBank:SetAction( "ApplicationLoadBankScene" )
	
	local btnPlant = guienv:AddButton( 517, 326, 517 + 212, 326 + 161, cityWindow:Self(), -1, "")
	btnPlant:SetImage( 0, 0, 212, 161, "media/buttons/plant_normal.tga" )
	btnPlant:SetHoveredImage( 0, 0, 212, 161, "media/buttons/plant_select.tga" )	
	btnPlant:SetPressedImage( 0, 0, 212, 161, "media/buttons/plant_select.tga" )	
	btnPlant:SetAction( "ApplicationLoadPlantScene" )
	
	local btnPizza = guienv:AddButton( 105, 42, 105 + 219, 42 + 171, cityWindow:Self(), -1, "")
	btnPizza:SetImage( 0, 0, 219, 171, "media/buttons/pizza_normal.tga" )
	btnPizza:SetHoveredImage( 0, 0, 219, 171, "media/buttons/pizza_select.tga" )	
	btnPizza:SetPressedImage( 0, 0, 219, 171, "media/buttons/pizza_select.tga" )	
	btnPizza:SetAction( "ApplicationLoadPizzaScene" )
	
	local btnUniver = guienv:AddButton( 0, 287, 287, 287 + 273, cityWindow:Self(), -1, "" )
	btnUniver:SetImage( 0, 0, 287, 273, "media/buttons/univer_normal.tga" )
	btnUniver:SetHoveredImage( 0, 0, 287, 273, "media/buttons/univer_select.tga" )	
	btnUniver:SetPressedImage( 0, 0, 287, 273, "media/buttons/univer_select.tga" )	
	btnUniver:SetAction( "ApplicationLoadUniverScene" )

end

function ApplicationLoadBankScene()
	--[[sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpBankScene.irr" )
	
	local loan = sceneManager:GetSceneNodeByName( "loanNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( loan:Self() )
	loan:SetTriangleSelector( selector )
	sceneManager:SetMarkText( loan:Self(), "loan" )
	
	local deposit = sceneManager:GetSceneNodeByName( "depositNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( deposit:Self() )
	deposit:SetTriangleSelector( selector )
	sceneManager:SetMarkText( deposit:Self(), "deposit" )
	
	local akcii = sceneManager:GetSceneNodeByName( "acciiNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( akcii:Self() )
	akcii:SetTriangleSelector( selector )
	sceneManager:SetMarkText( akcii:Self(), "akcii" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitBank" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:SetSelectedNode( nil )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
	--]]
end

function ApplicationLoadShopScene()
    --[[sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpShopScene.irr" )

	--���� � �������
	--���-���� ������
	--���-���� �� ��� �����
	--������� �������
	local gameInSale = sceneManager:GetSceneNodeByName( "gameInSaleNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameInSale:Self() )
	gameInSale:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameInSale:Self(), "gameInSale" )
	LogScript( "gameInSale find" )	
	
	local topListMonth = sceneManager:GetSceneNodeByName( "topListMonthNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListMonth:Self() )
	topListMonth:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListMonth:Self(), "topListMonth" )
	LogScript( "topListMonth find" )	

	local topListAllTime = sceneManager:GetSceneNodeByName( "allTimeTopListNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListAllTime:Self() )
	topListAllTime:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListAllTime:Self(), "topListAllTime" )
	LogScript( "topListAllTime find" )	

	local gameJournals = sceneManager:GetSceneNodeByName( "gameJournalsNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameJournals:Self() )
	gameJournals:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameJournals:Self(), "gameJournals" )
	LogScript( "gameJournals find" )	
	
	local exitN = sceneManager:GetSceneNodeByName( "exitShopNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	LogScript( "exitN find" )	
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnShopScene" )
	--]]
end

function ApplicationLoadOfficeScene()
	cityWindow:SetVisible( false )
	
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/reception.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		receptionWindow:GetCloseButton():SetVisible( false )
		receptionWindow:SetDraggable( false )
		
		--local btn = guienv:AddButton()
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_RECEPTION )
	
	local btnDirector = guienv:AddButton( 448, 242, 448 + 85, 242 + 175, receptionWindow:Self(), -1, "")
	btnDirector:SetImage( 0, 0, 85, 175, "media/buttons/director_normal.tga" )
	btnDirector:SetHoveredImage( 0, 0, 85, 175, "media/buttons/director_select.tga" )	
	btnDirector:SetPressedImage( 0, 0, 85, 175, "media/buttons/director_select.tga" )	
	btnDirector:SetAction( "ApplicationLoadDirectorCabinetScene" )
		
	--[[local newProj = sceneManager:GetSceneNodeByName( "createNewProjectNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( newProj:Self() )
	newProj:SetTriangleSelector( selector )
	sceneManager:SetMarkText( newProj:Self(), "createNewProject" )
	
	local showEmployers = sceneManager:GetSceneNodeByName( "employerManageNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showEmployers:Self() )
	showEmployers:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showEmployers:Self(), "employerManageNode" )
	
	local showProjectManager = sceneManager:GetSceneNodeByName( "projectManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showProjectManager:Self() )
	showProjectManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showProjectManager:Self(), "projectManagerNode" )
	
	local showInventionManager = sceneManager:GetSceneNodeByName( "inventionManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showInventionManager:Self() )
	showInventionManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showInventionManager:Self(), "inventionManagerNode" )
		
	local exitN = sceneManager:GetSceneNodeByName( "exitOfficeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
	--]]
end

function ApplicationLoadPlantScene()
	--[[sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpPlantScene.irr" )

	local createBoxNode = sceneManager:GetSceneNodeByName( "gameBoxManagerNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( createBoxNode:Self() )
	createBoxNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( createBoxNode:Self(), "createBoxNode" )
	
	local plnatNode = sceneManager:GetSceneNodeByName( "plantCeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plnatNode:Self() )
	plnatNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plnatNode:Self(), "plantCeNode" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitPlantNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnPlantScene" )
	--]]
end

function ApplicationLoadUniverScene()
	cityWindow:SetVisible( false )
	
	if univerWindow then
		univerWindow:SetVisible( true )
	else
		univerWindow = guienv:AddWindow( "media/univer_dvor.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		univerWindow:GetCloseButton():SetVisible( false )
		univerWindow:SetDraggable( false )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_UNIVER )
	
	local btnStuffUp = guienv:AddButton( 122, 320, 122 + 324, 329 + 130, univerWindow:Self(), -1, "")
	btnStuffUp:SetImage( 0, 0, 324, 130, "media/buttons/stuffPlate_normal.png" )
	btnStuffUp:SetHoveredImage( 0, 0, 324, 130, "media/buttons/stuffPlate_select.png" )	
	btnStuffUp:SetPressedImage( 0, 0, 324, 130, "media/buttons/stuffPlate_select.png" )	
	btnStuffUp:SetAction( "sworkCreateEmployersWindow" )
	
	local btnOutsourcing = guienv:AddButton( 612, 300, 612 + 211, 300 + 156, univerWindow:Self(), -1, "")
	btnOutsourcing:SetImage( 0, 0, 211, 156, "media/buttons/outsorcing_normal.png" )
	btnOutsourcing:SetHoveredImage( 0, 0, 211, 156, "media/buttons/outsorcing_select.png" )	
	btnOutsourcing:SetPressedImage( 0, 0, 211, 156, "media/buttons/outsorcing_select.png" )	
	btnOutsourcing:SetAction( "sworkCreateWindowOutsorcing" )
	
	local btnExit = guienv:AddButton( 455, 315, 455 + 211, 315 + 156, univerWindow:Self(), -1, "")
	btnExit:SetImage( 0, 0, 137, 187, "media/buttons/univerExit_normal.png" )
	btnExit:SetHoveredImage( 0, 0, 137, 187, "media/buttons/univerExit_select.png" )	
	btnExit:SetPressedImage( 0, 0, 137, 187, "media/buttons/univerExit_select.png" )	
	btnExit:SetAction( "sworkWindowUpEmployerClose" )

end

function ApplicationLoadDirectorCabinetScene( ptr )
	receptionWindow:SetVisible( false )
	
	if directorCabinetWindow then
		directorCabinetWindow:SetVisible( true )
	else
		directorCabinetWindow = guienv:AddWindow( "media/director_cabinet.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		directorCabinetWindow:SetDraggable( false )
		
		local closeBtn = directorCabinetWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_DIRECTORS_ROOM )
	
	local btnNewProject = guienv:AddButton( 0, 227, 0 + 197, 227 + 165, directorCabinetWindow:Self(), -1, "")
	btnNewProject:SetImage( 0, 0, 197, 165, "media/buttons/newProject_normal.png" )
	btnNewProject:SetHoveredImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetPressedImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetAction( "sworkCreateWindowWizardProject" )

end
