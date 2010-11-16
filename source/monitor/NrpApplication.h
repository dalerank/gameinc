#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME BANK( "bank" );
OPTION_NAME PROFILENAME( "profileName" );
OPTION_NAME PROFILECOMPANY( "profileCompany" );
OPTION_NAME WORKDIR( "workDir" );
OPTION_NAME SAVEDIR( "saveDir" );
OPTION_NAME SAVEDIR_INVENTIONS( "saveDirInvention" );
OPTION_NAME SAVEDIR_COMPANIES( "saveDirCompanies" );
OPTION_NAME SAVEDIR_DEVPR( "saveDirDevProjects" );
OPTION_NAME SAVEDIR_GAMES( "saveDirGames" );
OPTION_NAME SAVEDIR_PROJECTS( "saveDirProjects" );
OPTION_NAME SAVEDIR_ENGINES( "saveDirEngines" );
OPTION_NAME SAVEDIR_PLANT( "saveDirPlant" );
OPTION_NAME SAVEDIR_USERS( "saveDirUsers" );
OPTION_NAME SAVEINI_PROFILE( "saveIniProfile" );
OPTION_NAME SAVEDIR_PROFILE( "saveDirProfile" );
OPTION_NAME SAVEDIR_TECHS( "saveDirTechs" );
OPTION_NAME CURRENTTIME( "currentTime" );
OPTION_NAME BOXADDONNUMBER( "boxAdonNumber" );
OPTION_NAME MARKETGAMENUMBER( "marketGameNumber" );
OPTION_NAME PLAYERCOMPANY( "playerCompany" );
OPTION_NAME MINIMUM_USER_SALARY( "minimumUserSalary" );
OPTION_NAME PDA( "pda" ); 
OPTION_NAME SYSTEMINI( "systemIni" );
OPTION_NAME GAME_TIME( "objectGameTime" );
OPTION_NAME PAUSEBTWSTEP( "pausebtwstep" );

class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
class CNrpGame;
class CNrpDiskMachine;
class CNrpGameEngine;
class CNrpInvention;
class INrpDevelopProject;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	friend class CNrpGameTime;
public:
	static CNrpApplication& Instance();

	void ResetData();

	void Save();
	void Load( const NrpText& profileName, const NrpText& companyName );
	void CreateProfile( const NrpText& profileName, const NrpText& companyName );

	COMPANIES& GetCompanies();
	CNrpCompany* GetCompany( const NrpText& companyName );
	CNrpCompany* GetCompany( u32 index );
	int AddCompany( CNrpCompany* company );

	int AddUser( IUser* user );
	int RemoveUser( IUser* user );
	void CreateNewFreeUsers();
	IUser* GetUser( u32 index );
	IUser* GetUser( const NrpText& name );

	void AddDevelopProject( INrpDevelopProject* project );
	void RemoveDevelopProject( const NrpText& name );
	INrpDevelopProject* GetDevelopProject( const NrpText& name ); 

	void AddGameEngine( nrp::CNrpGameEngine* ptrEngine );
	CNrpGameEngine* GetGameEngine( const NrpText& name );
	void RemoveGameEngine( nrp::CNrpGameEngine* ptrEngine );

	CNrpGame* GetGame( const NrpText& name );
	CNrpGame* GetGame( u32 index );
	void AddGame( CNrpGame* ptrGame );

	void AddProject( nrp::INrpProject* project );
	INrpProject* GetProject( const NrpText& name );

	int GetTechsNumber() { return technologies_.size(); }
	CNrpTechnology* GetTechnology( int index );
	CNrpTechnology* GetTechnology( const NrpText& name );

	void AddTechnology( CNrpTechnology* ptrTech );
	void RemoveTechnology( CNrpTechnology* ptrTech );

	void UpdateGameRatings( CNrpGame* ptrGame, bool firstTime=false );

	CNrpTechnology* GetBoxAddon( size_t index ) { return index < boxAddons_.size() ? boxAddons_[ index ] : NULL; }
	CNrpTechnology* GetBoxAddon( const NrpText& name );
	bool AddBoxAddon( CNrpTechnology* tech );

	void AddGameToMarket( CNrpGame* game );

	CNrpRetailer* GetRetailer( const NrpText& name );
	void RemoveRetailer( const NrpText& name );

	NrpText GetFreeInternalName( CNrpGame* game );
	CNrpScreenshot* GetScreenshot( const NrpText& name );
	void LoadScreenshots( const NrpText& fileName );
	float GetGameGenreInterest( CNrpGame* game );

	void AddInvention( const NrpText& startTech, CNrpCompany* parentCompany );
	void InventionFinished( CNrpInvention* ptrInvention );
	void InventionCanceled( CNrpInvention* ptrInvention );
	CNrpInvention* GetInvention( const NrpText& name, const NrpText& companyName );
	static NrpText ClassName();
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES _companies;
	SCREENSHOTS _screenshots;
	USERS users_;
	TECHS technologies_;					//������ ��� ���������� �������� ����
	INVENTIONS inventions_;
	TECHS boxAddons_;
	GAMES games_;
	RETAILERS retailers_;
	ENGINES engines_;
	PROJECTS _projects;
	DEVPROJECTS _devProjects;

	void _BeginNewHour();
	void _BeginNewDay();
	void _BeginNewMonth();
	IUser* CreateRandomUser_( NrpText userType );
	void UpdateMarketGames_();
	int GetFreePlatformNumberForGame_( CNrpGame* game );
	int GetSalesNumber_( CNrpGame* game );
	void UpdateInvention_();

	void _LoadUsers( const NrpText& iniFile );
	void _InitialyzeSaveDirectories( const NrpText& profileName );
	void _UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating );
	void _CreateDirectoriesMapForSave();
};

}//namespace nrp