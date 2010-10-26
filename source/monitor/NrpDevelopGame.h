#pragma once
#include "INrpDevelopProject.h"

namespace nrp
{
CLASS_NAME CLASS_DEVELOPGAME("CNrpDevelopGame");


class CNrpProjectModule;
class CNrpGameProject;
class CNrpCompany;
class CNrpScenario;
class CNrpLicense;

class CNrpDevelopGame : public INrpDevelopProject
{
	typedef std::vector< CNrpProjectModule* > MODULE_LIST;
	CNrpDevelopGame() : INrpDevelopProject( CLASS_DEVELOPGAME, "" ) {}; 

public:
	CNrpDevelopGame( const std::string& name, CNrpCompany* ptrCompany );
	CNrpDevelopGame( CNrpGameProject* nProject, CNrpCompany* ptrCompany );
	CNrpDevelopGame( const std::string& fileName );

	std::string Save( const std::string& folderSave );
	void Load( const std::string& loadFolder );
	CNrpProjectModule* GetGenre( size_t index );
	CNrpProjectModule* GetModule( size_t index );
	CNrpProjectModule* GetModule( const char* name );

	void CalculateCodeVolume();
	bool IsReady();

	~CNrpDevelopGame(void);

	static std::string ClassName() { return CLASS_DEVELOPGAME; }
private:
	MODULE_LIST _modules;

	void InitializeOptions_( const std::string& name );
	void ModuleFinished( CNrpProjectModule* module );
	void FindPlaformsAndLanguages_();
};

}//end namespace nrp