#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
typedef enum { TS_UNKNOWN=0, TS_READY, TS_INDEVELOP, TS_PROJECT, TS_COUNT } TECH_STATUS;
const NrpText TECH_STATUS_NAME[TS_COUNT] = { L"unknown", L"ready", L"indevelop", L"project" };

class CNrpUser;
class CNrpCompany;
class CNrpInvention;

OPTION_NAME BASE_CODE( L"baseCode" );
OPTION_NAME ENGINE_CODE( L"engineCode" );
OPTION_NAME LASTWORKER( L"lastWorker" );
OPTION_NAME HAVELIDER( L"haveLider" );
OPTION_NAME INTEREST( L"interest" );
OPTION_NAME NEXTTECHNUMBER( L"nexttechnumber" );
OPTION_NAME REQUIRETECH( L"requireTech" );
OPTION_NAME STATUS( L"status" );

class CNrpTechnology : public INrpProject
{
public:
	CNrpTechnology( PROJECT_TYPE typen, const CLASS_NAME& className=CNrpTechnology::ClassName() );
	CNrpTechnology( CNrpInvention& invention );
	CNrpTechnology( const NrpText& fileTech );
	~CNrpTechnology(void);

	void SetEngineTechRequire( const NrpText&, int valuel );
	void SetEmployerSkillRequire( const NrpText&, int valuel );

	int GetEngineTechRequire( const NrpText& techName );
	int GetEployerSkillRequire( const NrpText& skillName );

	const NrpText GetFutureTech( size_t index );
	void AddFutureTech( const NrpText& techName );

	float GetEmployerPosibility( CNrpUser* ptrUser );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	const KNOWLEDGE_MAP& GetTechRequires() { return _techRequires; }
	const KNOWLEDGE_MAP& GetSkillRequires() { return _skillRequires; }

	static NrpText ClassName();
protected:
	CNrpTechnology();
	CNrpTechnology( const CNrpTechnology& p );

	void _InitializeOptions();

	KNOWLEDGE_MAP _techRequires;
	KNOWLEDGE_MAP _skillRequires;
	STRINGS _futureTech;
}; 

typedef CNrpTechnology* PNrpTechnology;

}//namespace nrp