#pragma once
#include "IWorkingModule.h"

namespace nrp
{
class INrpDevelopProject;

class CNrpProjectModule : public IWorkingModule
{
public:
    static const NrpText saveTemplate;

	CNrpProjectModule( CNrpTechnology& pTech, INrpDevelopProject& pProject  );
	CNrpProjectModule( CNrpPlatform& platform, INrpDevelopProject& project );
	CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject& pProject );
	virtual ~CNrpProjectModule(void);

	int AddUser( CNrpUser& ptrUser );
	int RemoveUser( const NrpText& userName );
	bool  IsMyUser( CNrpUser& user );

	const USERS& GetUsers() const { return _users; }

 	void Update( CNrpUser& ptrUser, const NrpTime& time );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	static NrpText ClassName();

private:
	void  _InitializeOptions();
	float _GetWorkKoeffForUser( CNrpUser& ptrUser );

	//���������� ��������� ���������� , ��� ��������� ������ ����������� ����
	void  _UpdateProgress( NParam& rr );

	USERS _users;
};

typedef CNrpProjectModule* PNrpProjectModule;

}//end namespace nrp