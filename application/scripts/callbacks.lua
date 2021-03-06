labelSpeed = nil

function sworkAppDayChange( ptr )
	updates.CheckNewEvents()
end

function sworkAppMonthChange()
	guienv:MessageBox( "������ ����� ���������� ���������� ��. ��� ����� ������ ��������� �����.", false, false, button.CloseParent, button.NoFunction )

	sceneManager:DrawProgress( 10, "��������� ����� �����" )
	applic:CreateNewFreeUsers()
	
	sceneManager:DrawProgress( 30, "���� ����� ��������������" )
	updates.CheckNewReklames( true )
	
	sceneManager:DrawProgress( 40, "���� ����� ����������" )
	updates.CheckNewTechs() 
	
	sceneManager:DrawProgress( 60, "���� ����� �������� ��� �������" )
	updates.CheckGameBoxAddons()
	
	sceneManager:DrawProgress( 80, "���� ����� ����" )
	updates.CheckNewGames()
	
	--������� ������ � �������� ��������
	updates.UpdatePlatformSales()
	
	PayMoneyToInventions()
	PaySalaryToWorkers()
end

function PaySalaryToWorkers()
	local company = applic.playerCompany
	
	local needMoney = 0
	local user = nil
	local salary = 0
	
	--������� ����� ��� ������� ��������
	for i=1, company.userNumber do
		user = company:GetUser( i-1 )
		--�������� ��������
		salary = user:GetParam( "salary" )
		user:AddParam( "balance", salary, false )
		needMoney = needMoney + salary
		
		--�������� �������� ������
		salary = user:GetParam( "lastAward" )
		user:SetParam( "lastAward", salary / 2 )
	end
	
	company:AddBalance( "������� ������� �����������", -needMoney )
end

function PayMoneyToInventions()
	local company = applic.playerCompany
	for index=1, company.inventionNumber do
		local invention = company:GetInvention( index-1 )
					
		company:AddBalance( "���������� � ��������", -invention:GetMonthPay() )
		invention:ClearMonthPay()
	end
end

function sworkAppYearChange( ptr )

end

function sworkInventionFinished( ptr )
	local inv = CLuaTech( ptr )
	pda.Show( "��������� ������ ��� ������������ "..inv.name )
end

function sworkPlayerCompanyReadyProject( company, ptrGm )
	local game = CLuaGame( ptrGm )
	pda.Show( "��������� ������ ��� �������� "..game.name )
end

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	
	sceneManager:DrawProgress( 90, "�������� ������ PDA" )
	applic.pda:Save()
	
	sceneManager:DrawProgress( 100, "�������� ������ ������������" )
	applic.plant:Save()
	
	sceneManager:DrawProgress( 100, "���������� �������" )	
	NrpApplicationClose()
end

function sworkModuleFinished( project, ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "��������� ������ ��� ������� "..mod.name.." ������� "..mod.parent.name )
end

function sworkModuleTestedFinished( project, ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "��������� ������������ ������ "..mod.name.." ������� "..mod.parent.name )	
end

function sworkUserMarketUpdated()
	pda.Show( "���������� �� ����� �����" )
end

function sworkReklameFinished( ptrReklame )
	local reklame = CLuaReklame( ptrReklame )
	local rtobject = CLuaProject( reklame.reklameObject )
	pda.Show( "����������� ��������� �������� " .. reklame.name .. " ��� " .. rtobject.name )
end

function sworkProduceFinished( ptrWork )
	local work = CLuaPlantWork( ptrWork )
	pda.Show( "�� ������ ��������� ������������ "..work.numberDisk.. " ������� � ����� "..work.game.name )
end

local function localChangeSpeed( keyInput )
	if labelSpeed then labelSpeed:Remove() end
	
	if keyInput == 0xBB then 
		if applic.speed > 100 then applic.speed = (applic.speed - 100) end
	else
		if applic.speed < 1000 then applic.speed = (applic.speed + 100) end
	end
	
	local dd = ( 1000 - applic.speed ) / 100
	
	local text = ""
	if dd ~= 0 then
		text = "�������� ���� " .. dd
	else
		text = "�����"
	end
	
	labelSpeed = guienv:AddLabel( text, "25%", "45%", "50%+", "20%+", -1, guienv.root )
	labelSpeed:SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )
	labelSpeed.font = "font_28"
							
	--LogScript( "labelSpeed:Update to "..dd )
	guienv:AddBlenderAnimator( labelSpeed, 255, 10, 2000, false, true, false )
end

function sworkKeyboardEvent( _, ptr )
	local event = CLuaEvent( ptr )
	local keyInput = event.key

	--LogScript( "key event "..keyInput )
	if  event.keyDown and ( keyInput == 0xBD or keyInput == 0xBB ) then
		localChangeSpeed( keyInput )
	end
end

function sworkNewGameOnMarket( _, ptr )
	local game = CLuaGame( ptr )
	--������� ������������ ��� ����� ����� ����
	pda.Show( "�� ����� ��������� ����� ���� "..game.name )
	
	--����� ���� �������� �������������� �������� �������� �� 10%
	if game.company then
		game.company.invexp = game.company.invexp * 1.1 
	end
end