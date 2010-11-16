#pragma once
#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME CEO("ceo");
OPTION_NAME ENGINES_NUMBER( "engineNumber" );
OPTION_NAME PROJECTNUMBER( "projectNumber" );
OPTION_NAME GAMENUMBER( "gameNumber" );
OPTION_NAME OBJECTSINPORTFELLE( "objectInPortfelle" );
OPTION_NAME DEVELOPPROJECTS_NUMBER( "developProjectsNumber" );
OPTION_NAME INVENTIONSNUMBER( "inventionsNumber" );

class CNrpCompany : public INrpConfig
{
public:
	CNrpCompany( const NrpText& name, IUser* ceo );
	CNrpCompany( const NrpText& fileName );
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index ) const;
	CNrpGameEngine* GetGameEngine( const NrpText& name ) const;

	CNrpTechnology* GetTechnology( u32 index ) const;
	CNrpTechnology* GetTechnology( const NrpText& name ) const;
	void AddTechnology( CNrpTechnology* tech );
	INrpConfig* GetFromPortfelle( size_t index ) const;
	void RemoveFromPortfelle( const INrpConfig* ptrObject );
	void AddToPortfelle( INrpConfig* const ptrObject );
	float GetUserModificatorForGame( CNrpGame* game );

	void AddDevelopProject( INrpDevelopProject* ptrDevProject );
	INrpDevelopProject* GetDevelopProject( const NrpText& name ) const;
	INrpDevelopProject* GetDevelopProject( size_t index ) const;
	void RemoveDevelopProject( const NrpText& name );

	void AddProject( INrpProject* ptrProject );
	void RemoveProject( const NrpText& name );
	INrpProject* GetProject( const NrpText& name ) const;
	INrpProject* GetProject( size_t index ) const;

	void AddInvention( CNrpInvention* const inv );
	void InventionReleased( const CNrpInvention* inv );
	CNrpInvention* GetInvention( const NrpText& name );
	CNrpInvention* GetInvention( int index );
	void RemoveInvention( CNrpInvention* inv );

	void AddUser( IUser* user );
	void RemoveUser( const NrpText& name );
	IUser* GetUser( int index ) const;
	IUser* GetUser( const NrpText& name ) const;
	
	CNrpGame* GetGame( const NrpText& gameName ) const;
	CNrpGame* GetGame( size_t index ) const;
	CNrpGame* CreateGame( CNrpDevelopGame* ptrProject );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );
	void BeginNewMonth( const SYSTEMTIME& time );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	static NrpText ClassName();
private:
	PROJECTS _projects; 
	ENGINES _engines;
	TECHS _technologies;
	DEVPROJECTS _devProjects;
	GAMES _games;
	USERS _employers;
	OBJECTS _portfelle;
	INVENTIONS _inventions;

	void _PaySalaries();
	void _UpdateGameProjectState();
	void _InitialyzeOptions();
	void _LoadArray( const NrpText& section, const NrpText& fileName, const NrpText& condition );
}; 

typedef CNrpCompany* PNrpCompany;
}//namespace nrp