#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME PROFILENAME( L"profileName" );
OPTION_NAME PROFILECOMPANY( L"profileCompany" );
OPTION_NAME WORKDIR( L"workDir" );
OPTION_NAME SAVEDIR( L"saveDir" );
OPTION_NAME SAVEDIR_INVENTIONS( L"saveDirInvention" );
OPTION_NAME SAVEDIR_COMPANIES( L"saveDirCompanies" );
OPTION_NAME SAVEDIR_DEVPR( L"saveDirDevProjects" );
OPTION_NAME SAVEDIR_GAMES( L"saveDirGames" );
OPTION_NAME SAVEDIR_PLATFORMS( L"platforms" );
OPTION_NAME SAVEDIR_PROJECTS( L"saveDirProjects" );
OPTION_NAME SAVEDIR_ENGINES( L"saveDirEngines" );
OPTION_NAME SAVEDIR_PLANT( L"saveDirPlant" );
OPTION_NAME SAVEDIR_USERS( L"saveDirUsers" );
OPTION_NAME SAVEINI_PROFILE( L"saveIniProfile" );
OPTION_NAME SAVEDIR_PROFILE( L"saveDirProfile" );
OPTION_NAME SAVEDIR_TECHS( L"saveDirTechs" );
OPTION_NAME SAVEDIR_BRIDGE( L"saveDirBridge" );
OPTION_NAME SAVEDIR_BANK( L"saveDirBank" );
OPTION_NAME SOUND_ENGINE( L"soundEngine" );
OPTION_NAME CURRENTTIME( L"currentTime" );
OPTION_NAME BOXADDONNUMBER( L"boxAdonNumber" );
OPTION_NAME MARKETGAMENUMBER( L"marketGameNumber" );
OPTION_NAME PLAYERCOMPANY( L"playerCompany" );
OPTION_NAME MINIMUM_USER_SALARY( L"minimumUserSalary" );
OPTION_NAME PDA( L"pda" ); 
OPTION_NAME SYSTEMINI( L"systemIni" );
OPTION_NAME GAME_TIME( L"objectGameTime" );
OPTION_NAME PAUSEBTWSTEP( L"pausebtwstep" );
OPTION_NAME INFLATION( L"infaltion" );
OPTION_NAME PROFIT_TAX( L"profit_tax" );
OPTION_NAME DEV_FORCE( L"dev_force" );

class CNrpUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
class CNrpDiskMachine;
class CNrpGameEngine;
class CNrpInvention;
class INrpDevelopProject;
class CNrpBridge;
class IniFile;
class CNrpLaborMarket;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	friend class CNrpGameTime;
public:
    static const NrpText oldTemplate;
    static const NrpText bakTemplate;

	static CNrpApplication& Instance();

	void ResetData();

	void Save();
	void Load( const NrpText& profileName, const NrpText& companyName );
	void CreateProfile( const NrpText& profileName, const NrpText& companyName );

	COMPANIES& GetCompanies();
	CNrpCompany* GetCompany( const NrpText& companyName );
	CNrpCompany* GetCompany( u32 index );
	int AddCompany( CNrpCompany* company );

	void AddDevelopProject( INrpDevelopProject* project );
	void RemoveDevelopProject( const NrpText& name );
	INrpDevelopProject* GetDevelopProject( const NrpText& name ); 

	void AddProject( nrp::INrpProject* project );
	INrpProject* GetProject( const NrpText& name );

	int GetTechsNumber() { return _technologies.size(); }
	CNrpTechnology* GetTechnology( int index );
	CNrpTechnology* GetTechnology( const NrpText& name );
	const TECHS& GetTechnologies() { return _technologies; }

	void AddTechnology( CNrpTechnology* ptrTech );
	void RemoveTechnology( CNrpTechnology* ptrTech );

	CNrpTechnology* GetBoxAddon( size_t index ) { return index < _boxAddons.size() ? _boxAddons[ index ] : NULL; }
	CNrpTechnology* GetBoxAddon( const NrpText& name );
	bool AddBoxAddon( CNrpTechnology* tech );

	void AddInvention( const NrpText& startTech, CNrpCompany& parentCompany );
	void InventionFinished( CNrpInvention& ptrInvention );
	void InventionCanceled( CNrpInvention* ptrInvention );
	CNrpInvention* GetInvention( const NrpText& name, const NrpText& companyName );
	void Init();

	static NrpText ClassName();

    static const NrpText saveTemplate;
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES _companies;
	TECHS _technologies;					//������ ��� ���������� �������� ����
	INVENTIONS _inventions;
	TECHS _boxAddons;
	PROJECTS _projects;
	DEVPROJECTS _devProjects;
	

	void _BeginNewHour();
	void _BeginNewDay();
	void _BeginNewMonth();
	void _UpdateInvention();

	void _InitialyzeSaveDirectories( const NrpText& profileName );
	void _CreateDirectoriesMapForSave();
    void _LoadCompanies( IniFile& ini );
    void _LoadDevProjects( IniFile& ini );
};

#define _nrpApp CNrpApplication::Instance()

}//namespace nrp