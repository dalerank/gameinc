local company = nil

local windowShop = nil
local listboxGames = CLuaListBox( nil )
local buttonAnonceGame = CLuaButton( nil )
local imageGamePreview = nil
local labelGameName = nil
local labelLastMonthSale = nil
local labelCurrentMonthSale = nil
local labelAllTimeSale = nil
local prgRating = CLuaProgressBar( nil )
local btnDecreaseGamePrice = CLuaButton( nil )
local labelGamePrice = nil
local btnIncreaseGamePrice = CLuaButton( nil )
local listboxCompanyGame = CLuaListBox( nil )
local anoncePictureFlow = nil
local windowAnonce = nil
local selectedGame = nil
local lastTimeParamsUpdate = GetTickCount()
local width = 800
local height = 600

local function localFillListboxGame()
	listboxGames:Clear()
	
	local game = nil
	for i=1, applic:GetGamesNumber() do
		game = applic:GetGame( i-1 )
		if game:IsSaling() then
			listboxGames:AddItem( game:GetName(), game:Self() )
		end
	end
end

local function localUpdateCurrentGameParams()
	labelGameName:SetText( "��������: " .. selectedGame:GetName() )
	labelLastMonthSale:SetText( "������ �� ������� �����:" .. selectedGame:GetLastMonthSales() )
	labelCurrentMonthSale:SetText( "������ �� ���� �����:" .. selectedGame:GetCurrentMonthSales() )
	labelAllTimeSale:SetText( "������ �� ��� �����:" .. selectedGame:GetAllTimeSales() )
	--prgRating:SetPos( selectedGame:GetCurrentQuality() ) 
	labelGamePrice:SetText( "����:" .. selectedGame:GetPrice() )
	
	if selectedGame:GetCompany() == company:Self() then
		btnDecreaseGamePrice:SetVisible( true )
		btnIncreaseGamePrice:SetVisible( true )		
	else
		btnDecreaseGamePrice:SetVisible( false )
		btnIncreaseGamePrice:SetVisible( false )			
	end
end

function sworkGameInSaleWindowListboxChanged( ptr )
    if ptr == listboxGames:Self() then
		selectedGame:SetObject( listboxGames:GetSelectedObject() )
		localUpdateCurrentGameParams()
    end
end

function sworkGameInSaleUpdateGameParams( ptr )
	
end

function sworkWindowShopDecreaseGamePrice( ptr )
	selectedGame:SetPrice( selectedGame:GetPrice() - 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function sworkWindowShopIncreaseGamePrice( ptr )
	selectedGame:SetPrice( selectedGame:GetPrice() + 1 )
	labelGamePrice:SetText( "#TRANSLATE_TEXT_PRICE:" .. selectedGame:GetPrice() )
end

function sworkCreateGameInSaleWindow( ptr )
	company = applic:GetPlayerCompany()
	windowShop = guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() )
	
	windowShop:AddLuaFunction( GUIELEMENT_LBXITEM_SELECTED, "sworkGameInSaleWindowListboxChanged" )
	windowShop:AddLuaFunction( GUIELEMENT_AFTER_DRAW, "sworkGameInSaleUpdateGameParams" )
	
	--������� ���� � ����������
	--� ��������� �������� ������ ���, ������� ��� � �������
	listboxGames:SetObject( guienv:AddListBox( 20, 20, width / 2 - 10, height - 20, -1, windowShop:Self() ) ) 
	localFillListboxGame()
	
	--���������� ������ "������������ ����", �� ������� ����� ��������� ���� �� �����
	buttonAnonceGame = guienv:AddButton( width / 2, height -50, width - 20, height - 20, 
								windowShop:Self(), -1, "������������ ����" )
	buttonAnonceGame:SetAction( "sworkWindowShopAnonceGame" )
	
	--���������� ����������� ���� ������ �� ������
	imageGamePreview = guienv:AddImage( width / 2 + 20, 20, width - 20, height / 2 - 20, windowShop:Self(), -1, "" )
	
	--���������� ��� ������������ �������� ��������� ������
	--�������� ����
	local hw = width / 2 + 20
	local hh = height / 2 + 20
	labelGameName = guienv:AddLabel( "��������: ", hw + 20, hh + 20, width - 20, hh + 40, -1, windowShop:Self() )
	
	--������ �� ������� �����
	labelLastMonthSale = guienv:AddLabel( "������ �� ������� �����:", hw + 20, hh + 50, 
												   width - 20, hh + 70, -1, windowShop:Self() )
	--������ �� ������� �����
	labelCurrentMonthSale = guienv:AddLabel( "������ �� ���� �����:", hw + 20, hh + 80, 
													  width - 20, hh + 100, -1, windowShop:Self() )
	--����� ������� �����
	labelAllTimeSale = guienv:AddLabel( "������ �� ��� �����:", hw + 20, hh + 110, 
													  width - 20, hh + 130, -1, windowShop:Self() )
	--������� ������� ����
	prgRating = guienv:AddProgressBar( windowShop:Self(), hw + 20, hh + 140, 
													width - 20, hh + 160, -1 )
	--prgRating:SetImage( )
	--prgRating:SetFillImage( )

	--���� ���� � ������������ �������� ����
	btnDecreaseGamePrice = guienv:AddButton( hw + 20, hh + 170, hw + 40, hh + 190, 
									windowShop:Self(), -1, "-" )
	btnDecreaseGamePrice:SetAction( "sworkWindowShopDecreaseGamePrice" ) 
									
	labelGamePrice = guienv:AddLabel( "#TRANSLATE_TEXT_PRICE:", hw + 40, hh + 170, 
													    width - 40, hh + 190, -1, windowShop:Self() )
													 
	btnIncreaseGamePrice = guienv:AddButton(  width - 40, hh + 170, width - 20, hh + 190, 
									windowShop:Self(), -1, "+" )
	btnIncreaseGamePrice:SetAction( "sworkWindowShopIncreaseGamePrice" ) 	
end

function sworkWindowShopAnonceGame( ptr )
	windowAnonce = guienv:AddWindow( "", width / 2 - 200, height / 2 - 120,
						  				 width / 2 + 200, height / 2 + 120, 
								         -1, windowShop:Self() ) 
	
	anoncePictureFlow = guienv:AddPictureFlow( 10, 10, 390, 200, -1, windowAnonce:Self() )
	anoncePictureFlow:SetPictureRect( 0, 0, 90, 90 )
	
	for i=1, company:GetGameNumber() do
		local game = company:GetGame( i-1 )
		
		if not game:IsSaling() then 
			anoncePictureFlow:AddItem( game:GetViewImage(), game:GetName(), game:Self() )
		end	
	end
	
	local btnOk = guienv:AddButton( 10, 240 - 40, 190, 240, windowAnonce:Self(), -1, "������ �������" )
	btnOk:SetAction( "sworkWindowShopStartGameSaling" )
	
	local btnCancel = guienv:AddButton( 210, 240 - 40, 390, 240, windowAnonce:Self(), -1, "�����" )
	btnCancel:SetAction( "sworkWindowShopCloseAnonceGame" )
end

function sworkWindowShopStartGameSaling( ptr )
	local game = anoncePictureFlow:GetSelectedObject()
	
	applic:AddGameToMarket( game )
	localFillListboxGame()
	sworkWindowShopCloseAnonceGame( ptr )	
end

function sworkCreateMonthTopListWindow( ptr )

end

function sworkCreateAllTimeTopListWindow( ptr )

end

function sworkWindowShopCloseAnonceGame( ptr )
	windowAnonce:Remove()
end