-- �������� ������ �����
require("logdefs")
require("elmid")
IncludeScript("swork_terrainConfig")
IncludeScript("swork_MainMenu")
IncludeScript("swork_wndCompanyInfo")
IncludeScript("swork_StoreWindow")
IncludeScript("swork_camera")
IncludeScript("swork_ObjectsLoading")
IncludeScript("swork_wndProjectWizard")
IncludeScript("swork_startDefinition")

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������������� ������ � ��")
-- �������� ��������������� �������
LogVideo("���� �������� ���������", 0xDEADBEEF)

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local app = CLuaApplication( NrpGetApplication() )

citySceneObjects = { } 
bankSceneObjects = { }
univerSceneObjects = { }
officeSceneObjects = { }

--�������� ������� ����� 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--���������� ������ � �����
AddCamera()
sceneManager:DrawProgress( 25 )

--���������� �������� � �����
AddTerrain()
sceneManager:DrawProgress( 50 )

--�������� �������� ����
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--�������� ������� �����
AddStoreWindow()
sceneManager:DrawProgress( 77 )

--�������� ���������
sceneManager:DrawProgress( 85 )

--�������� ����
sceneManager:AddSkyDomeSceneNode( "media/sky/panorama.jpg", 20, 12, 1, 2.0, 10000 )
sceneManager:DrawProgress( 90 )

--�������� ��������� �����
--AddGlobalMap()
sceneManager:DrawProgress( 95 )

AddObjectsToCityScene()
AddObjectsToBankScene()
AddObjectsToOfficeScene()
AddObjectsToUniverScene()
sceneManager:DrawProgress( 100 )

AddStartPlayerDef()
AddStartCompanyTechnology()
AddStartVideoContentTechnology()
AddStartSoundContentTechnology()
AddStartScenarioContentTechnology()
DebugFunctionCall( AddStartPublicTechnology )

sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )