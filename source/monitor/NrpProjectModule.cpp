#include "StdAfx.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "INrpDevelopProject.h"
#include "OpFileSystem.h"
#include "NrpPlatform.h"
#include "IniFile.h"
#include "math.h"

#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_PROJECTMODULE( "CNrpProjectModule" );

const NrpText CNrpProjectModule::saveTemplate = L".devmod";

CNrpProjectModule::CNrpProjectModule( CNrpTechnology& refTech, INrpDevelopProject& pProject )
	: IWorkingModule( PROJECT_TYPE( (int)refTech[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	_InitializeOptions();

	_self[ NAME ] = refTech[ NAME ];
	_self[ TECHGROUP ] = refTech[ TECHGROUP ];
	_self[ BASE_CODE ] = refTech[ BASE_CODE ];
	_self[ ENGINE_CODE ] = refTech[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = refTech[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = refTech[ TEXTURENORMAL ];
	_self[ LEVEL ] = refTech[ LEVEL ];
	_self[ PARENT ] = &pProject;
	_self[ QUALITY ] = refTech[ QUALITY ];

	CopyMapTo( _techRequires, refTech.GetTechRequires() );
	CopyMapTo( _skillRequires, refTech.GetSkillRequires() );
}

CNrpProjectModule::CNrpProjectModule( PROJECT_TYPE type, INrpDevelopProject& pProject )
	: IWorkingModule( type, CLASS_PROJECTMODULE )
{
	_InitializeOptions();

	_self[ TECHGROUP ] = static_cast< int >( type );
	_self[ PARENT ] = &pProject;
}

CNrpProjectModule::CNrpProjectModule( CNrpPlatform& platform, INrpDevelopProject& project )
: IWorkingModule( PROJECT_TYPE( (int)platform[ TECHGROUP ] ), CLASS_PROJECTMODULE )
{
	_InitializeOptions();

	_self[ NAME ] = NrpText::LuaString( "#PLATFORM_PORTING" ) + (NrpText)platform[ NAME ];
	_self[ TECHGROUP ] = platform[ TECHGROUP ];
	_self[ BASE_CODE ] = platform[ BASE_CODE ];
	_self[ ENGINE_CODE ] = platform[ ENGINE_CODE ];
	_self[ INTERNAL_NAME ] = platform[ INTERNAL_NAME ];
	_self[ TEXTURENORMAL ] = platform[ TEXTURENORMAL ];
	_self[ LEVEL ] = platform[ LEVEL ];
	_self[ PARENT ] = &project;
	_self[ QUALITY ] = platform[ QUALITY ];

	const TECHS& techs = platform.GetTechsList();
	for( u32 i=0; i < techs.size(); i++ )
	{
		const NrpText& name = (*techs[ i ])[ INTERNAL_NAME ];
		 _techRequires[ name ] = 100;
	}
}

CNrpProjectModule::~CNrpProjectModule(void)
{
}

void CNrpProjectModule::_UpdateProgress( NParam& rr ) { _self[ READYWORKPERCENT ] = (int)_self[ CODEPASSED ] / static_cast< float >( (int)_self[ CODEVOLUME ] ); }

void CNrpProjectModule::_InitializeOptions()
{
	RegProperty<CNrpUser*>( LASTWORKER, NULL );
	RegProperty<CNrpUser*>( COMPONENTLIDER, NULL );
	RegProperty( CODEVOLUME, 0 );
	RegProperty( CODEPASSED, 0 );
	RegProperty( ERRORNUMBER, 0 );
	RegProperty( USERNUMBER, 0 );
	RegProperty( MONEYONDEVELOP, 0 );
	UnregProperty( PARENT );
	RegProperty<INrpDevelopProject*>( PARENT, NULL );

	_self[ CODEPASSED ].AddNotification( "checkPercent", this, &CNrpProjectModule::_UpdateProgress );
}

bool CNrpProjectModule::IsMyUser( CNrpUser& user )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( *(_users[ i ]) == user )
			return true;

	return false;
}

int CNrpProjectModule::AddUser( CNrpUser& refUser )
{
	if( IsMyUser( refUser ) )
		return -1;

	_users.push_back( &refUser );
	_self[ USERNUMBER ] = static_cast< int >( _users.size() );

	return ( (float)_self[ READYWORKPERCENT ] < 1.f);
}

void CNrpProjectModule::Update( CNrpUser& refUser, const NrpTime& time )
{
	INrpDevelopProject* parent = _self[ PARENT ].As<INrpDevelopProject*>();
	assert( parent != NULL );

	//���� �� ����� �������� ���� ������, ��� ��� �������� ����
	if( (int)_self[ CODEPASSED ] < (int)_self[ CODEVOLUME] )
	{
		int reqSkill = 0;
		KNOWLEDGE_MAP::Iterator sIter = _skillRequires.getIterator();
		float teamKoeff = _GetWorkKoeffForUser( refUser );
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += refUser.GetSkill( sIter->getKey() );

		reqSkill = (std::max)( reqSkill, 10 );

		//����������� ������� ����� ��-�� ����, ��� ���������� ������ ������� 5, � ���������� ���� 
		//�������� � ��������� ������������������ ������� �� ������ �������,
		//���� ����� ������
		float genreSkill = (std::max)( 0.1f, teamKoeff * refUser.GetGenreExperience( _self[ INTERNAL_NAME ] ) / 100.f );
		float genrePref = (std::max)( 0.1f, teamKoeff * refUser.GetGenrePreferences( _self[ INTERNAL_NAME ] ) / 100.f );
		
		int codeGrow = static_cast< int >( reqSkill * (genrePref + genreSkill) );
		int codePassed = (int)_self[ CODEPASSED ] + codeGrow;
		//���������� ���� �� ������ ���� ������ �����������
		_self[ CODEPASSED ] = (std::min)( (int)_self[ CODEVOLUME ], codePassed );

		int quality = _self[ QUALITY ];

		//���������� �������� ������ � ������� ������� �� ����������
		_self[ QUALITY ] = static_cast< int >( (quality + (int)refUser[ WORK_QUALITY ]) / 2 );
		_self[ MONEYONDEVELOP ] += (int)refUser[ SALARY ] / (20*9); //������ �������� ���� �� 9 �����

		//���������� ���������� ������ ����������� �������� �� ���� ���� ����������
		//��� ������� �� ������ ���� ����������� �� ���� ������������ � ��� ���������� 
		//� ������� �� ������ 100 ����� ���� ���������� 1 ������ 
		//ln( 270 / 100 ) = 1 - ����� ��������� ���������� -> ���������� ������ �����������
		//���������� ������ ���������� ������ ������
		float koeffError = log( 270.f / (int)refUser[ MOOD ] ) / (std::max)( (int)refUser[ EXPERIENCE ], 10 );
		int errorNumber = static_cast< int >( codeGrow * koeffError );
		//���������� ������ �� ������ ���� �������������
		_self[ ERRORNUMBER ] += (std::max)( errorNumber, 0 );
	}

	if( _self[ READYWORKPERCENT ] >= 1.f )
		parent->ModuleFinished( *this );
}

float CNrpProjectModule::_GetWorkKoeffForUser( CNrpUser& ptrUser )
{
	float teamKoef[10] = { 1.f/*1*/, 4.f/*2*/, 3.f/*3*/, 2.f/*4*/, 1.f/*5*/, 0.8f/*6*/, 0.65f/*7*/, 0.5f/*8*/, 0.25f/*9*/, 0.1f/*10 and more*/};
	return teamKoef[ _users.size() >= 9 ? 9 : _users.size() ];
}

NrpText CNrpProjectModule::Save( const NrpText& saveFolder )
{
	OpFileSystem::CreateDirectory( saveFolder );

	NrpText fileName = saveFolder + Text( INTERNAL_NAME ) + saveTemplate;
	assert( !OpFileSystem::IsExist( fileName ) );

	INrpProject::Save( fileName );
	IniFile sv( fileName );

	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );

	sv.Save();
	return fileName;
}

void CNrpProjectModule::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );
		
	//�������� ���������� �� ����� ������
	INrpProject::Load( fileName );

	assert( (int)_self[ CODEPASSED ] <= (int)_self[ CODEVOLUME] );
	//�������!!!
	if( (int)_self[ CODEPASSED ] > (int)_self[ CODEVOLUME ] )
	{
		Log( HW ) << "��������� �������� ���������� CODEVOLUME < CODEPASSED ����� " << fileName << term;
		Log( HW ) << "�������� ����� �������, �������� " << (int)_self[ CODEPASSED ] + 10 << term;
		_self[ CODEVOLUME ] = (int)_self[ CODEPASSED ] + 10;
	}

	//�������� ������������ ����������
	IniFile rv( fileName );
	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );
}

int CNrpProjectModule::RemoveUser( const NrpText& userName )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( _users[ i ]->Equale( userName ) )
		{
			_users.erase( i );
			return 1;
		}

	return 0;
}

NrpText CNrpProjectModule::ClassName()
{
	return CLASS_PROJECTMODULE;
}

}//end namespace nrp