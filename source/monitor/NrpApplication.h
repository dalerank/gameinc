#pragma once
#include "nrpConfig.h"
#include "LuaFunctionality.h"
#include <vector>

namespace nrp
{

OPTION_NAME BANK( "bank" );
OPTION_NAME PROFILENAME( "profileName" );
OPTION_NAME PROFILECOMPANY( "profileCompany" );
OPTION_NAME FULLPATH( "fullPath" );

class CNrpCompany;
class IUser;
class INrpProject;
class CNrpGameEngine;
class CNrpTechnology;
	
class CNrpApplication : public INrpConfig, public ILuaFunctionality
{
	typedef std::vector< CNrpCompany* > COMPANIES_LIST;
	typedef std::vector< IUser* > USER_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
public:
	typedef enum { SPD_MINUTE=0, SPD_HOUR, SPD_DAY, SPD_MONTH, SPD_COUNT } SPEED;
	static CNrpApplication& Instance();

	void ResetData();

	void SaveProfile();
	void LoadProfile( std::string profileName, std::string companyName );
	void CreateProfile( std::string profileName, std::string companyName );

	COMPANIES_LIST& GetCompanies();
	CNrpCompany* GetCompany( std::string companyName ) const;
	int AddCompany( CNrpCompany* company );
	CNrpCompany* GetPlayerCompany();

	int AddUser( bool player, IUser* user );
	int RemoveUser( IUser* user );
	IUser* GetUser( int index );
	IUser* GetUser( std::string name );

	nrp::INrpProject* CreateGameProject( std::string name );

	nrp::CNrpGameEngine* CreateGameEngine( std::string name );

	bool UpdateTime();
	SYSTEMTIME& GetDateTime() { return time_; }

	int GetTechsNumber() const { return technologies_.size(); }
	CNrpTechnology* GetTechnology( int index ) const;
	void AddTechnology( CNrpTechnology* ptrTech );
	nrp::CNrpTechnology* CreateTechnology( int typeTech );
private:
	CNrpApplication(void);
	~CNrpApplication(void);

	COMPANIES_LIST companies_;
	USER_LIST users_;
	TECH_LIST technologies_;

	SPEED speed_;
	SYSTEMTIME time_;
	int lastTimeUpdate_;

	virtual void Load_( char* fileName ) {}
};

}//namespace nrp