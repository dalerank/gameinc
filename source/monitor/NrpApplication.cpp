#include "StdAfx.h"
#include "NrpApplication.h"
#include "NrpCompany.h"
#include "IUser.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "NrpBank.h"
#include "NrpAiUser.h"
#include "NrpPlayer.h"
#include "OpFileSystem.h"
#include "nrpEngine.h"
#include "PeopleName.h"
#include "NrpGame.h"
#include "NrpDiskMachine.h"
#include "NrpPlant.h"
#include "NrpGameBox.h"
#include "NrpRetailer.h"
#include "NrpScreenshot.h"
#include "NrpDevelopGame.h"
#include "NrpInvention.h"
#include "NrpActionType.h"
#include "NrpPda.h"
#include "NrpHistory.h"
#include "NrpGameTime.h"
#include "timeHelpers.h"
#include "IniFile.h"

#include <io.h>
#include <errno.h>
#include <OleAuto.h>

static nrp::CNrpApplication* globalApplication = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPAPPLICATION( "CNrpApplication" );

CNrpApplication::CNrpApplication(void) : INrpConfig( CLASS_NRPAPPLICATION, CLASS_NRPAPPLICATION )
{
	CreateValue<PNrpBank>( BANK, NULL );
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<int>( COMPANIESNUMBER, 0 );

	CreateValue<NrpText>( WORKDIR, "" );
	CreateValue<NrpText>( SAVEDIR, "save/" );
	CreateValue<NrpText>( SAVEDIR_INVENTIONS, "" );
	CreateValue<NrpText>( SAVEDIR_COMPANIES, "" );
	CreateValue<NrpText>( SAVEDIR_DEVPR, "" );
	CreateValue<NrpText>( SAVEDIR_GAMES, "" );
	CreateValue<NrpText>( SAVEDIR_PROJECTS, "" );
	CreateValue<NrpText>( SAVEDIR_ENGINES, "" );
	CreateValue<NrpText>( SAVEDIR_USERS, "" );
	CreateValue<NrpText>( SAVEDIR_PLANT, "" );
	CreateValue<NrpText>( SAVEINI_PROFILE, "" );
	CreateValue<NrpText>( SAVEDIR_PROFILE, "" );
	CreateValue<NrpText>( SAVEDIR_TECHS, "" );
	CreateValue<NrpText>( SYSTEMINI, "config/system.ini" );

	IniFile rv( GetString( SYSTEMINI ) );
	CreateValue<NrpText>( PROFILENAME, rv.Get( SECTION_OPTIONS, "currentProfile", NrpText( "dalerank" ) ) );
	CreateValue<NrpText>( PROFILECOMPANY, rv.Get( SECTION_OPTIONS, "currentCompany", NrpText( "daleteam" ) ) );

	CreateValue<SYSTEMTIME>( CURRENTTIME, SYSTEMTIME() );
	CreateValue<int>( BOXADDONNUMBER, 0 );
	CreateValue<int>( GAMENUMBER, 0 );
	CreateValue<int>( ENGINES_NUMBER, 0 );
	CreateValue<int>( DEVELOPPROJECTS_NUMBER, 0 );
	CreateValue<int>( PROJECTNUMBER, 0 );
	CreateValue<PNrpCompany>( PLAYERCOMPANY, NULL );
	CreateValue<int>( INVENTIONSNUMBER, 0 );
	CreateValue<int>( MINIMUM_USER_SALARY, 250 );
	CreateValue<CNrpPda*>( PDA, new CNrpPda() );
	CreateValue<CNrpGameTime*>( GAME_TIME, new CNrpGameTime( this ) );
	CreateValue<int>( PAUSEBTWSTEP, 100 );

	srand( GetTickCount() );
}

CNrpApplication::~CNrpApplication(void)
{
}

CNrpCompany* CNrpApplication::GetCompany( const NrpText& companyName )
{
	return FindByName< COMPANIES, CNrpCompany >( _companies, companyName );
}

CNrpCompany* CNrpApplication::GetCompany( u32 index )
{
	return index < _companies.size() ? _companies[ index ] : NULL;
}

COMPANIES& CNrpApplication::GetCompanies()
{
	return _companies;
}

int CNrpApplication::AddCompany( CNrpCompany* company )
{
	if( company )
		_companies.push_back( company );

	SetValue<int>( COMPANIESNUMBER, _companies.size() );
	PUser ceo = company->GetValue<PUser>( CEO );
	if( ceo && ceo->ObjectTypeName() == CNrpPlayer::ClassName() )
		SetValue<PNrpCompany>( PLAYERCOMPANY, company );

	return 1;
}

int CNrpApplication::AddUser( IUser* user )
{
	IUser* tmpUser = GetUser( user->GetString( NAME ) );
	assert( tmpUser == NULL );

	if( user && tmpUser == NULL )
		users_.push_back( user );

	SetValue<int>( USERNUMBER, users_.size() );
	return 1;
}


void CNrpApplication::AddProject( INrpProject* project )
{
	if( FindByName<PROJECTS, INrpProject>( _projects, project->GetString( NAME ) ) == NULL )
		_projects.push_back( project );
}

CNrpTechnology* CNrpApplication::GetTechnology( int index )
{
	return index < (int)technologies_.size() ?  technologies_[ index ] : NULL;
}

CNrpTechnology* CNrpApplication::GetTechnology( const NrpText& name )
{
	return FindByNameAndIntName<TECHS, CNrpTechnology>( technologies_, name );
}

void CNrpApplication::AddTechnology( CNrpTechnology* ptrTech )
{
	CNrpTechnology* tech = GetTechnology( ptrTech->GetString( INTERNAL_NAME) );
	if( !tech )
		technologies_.push_back( ptrTech );

	SetValue<int>( TECHNUMBER, technologies_.size() );
}

IUser* CNrpApplication::GetUser( u32 index )
{
	return index < users_.size() ? users_[ index ] : NULL;
}

IUser* CNrpApplication::GetUser( const NrpText& name )
{
	return FindByNameAndIntName<USERS, IUser>( users_, name );
}

int CNrpApplication::RemoveUser( IUser* user )
{
	for( u32 pos=0; pos < users_.size(); pos++ )
		if( users_[ pos ] == user )
		{
			delete users_[ pos ];
			users_.erase( pos );
			SetValue<int>( USERNUMBER, users_.size() );
			return 0;
		}

	return 1;
}

void CNrpApplication::_CreateDirectoriesMapForSave()
{
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PROFILE ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_INVENTIONS ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_COMPANIES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_DEVPR ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_GAMES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PROJECTS ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_ENGINES ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_PLANT ) );
	OpFileSystem::CreateDirectory( GetString( SAVEDIR_USERS ) );
}

void CNrpApplication::Save()
{
	NrpText prevSaveFolder = GetString( SAVEDIR ) + GetString( PROFILENAME ) + "Old/";
	NrpText profileIni = GetString( SAVEDIR_PROFILE ) + "profile.ini";

	OpFileSystem::Remove( prevSaveFolder );
	OpFileSystem::Move( GetString( SAVEDIR_PROFILE ), prevSaveFolder );

	_CreateDirectoriesMapForSave();

	assert( !OpFileSystem::IsExist( profileIni ) );
	IniFile sv( profileIni );

	INrpConfig::Save( profileIni );

	sv.Set( SECTION_OPTIONS, "currentProfile", GetString( PROFILENAME ) );
	sv.Set( SECTION_OPTIONS, "currentCompany", GetString( PROFILECOMPANY ) );
	
	for( u32 i=0; i < _companies.size(); i++ )
	{
		_companies[ i ]->Save( GetString( SAVEDIR_COMPANIES ) );
		sv.Set( SECTION_COMPANIES, CreateKeyCompany( i ), _companies[ i ]->GetString( NAME ) );
	}

	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		NrpText saveFile = _devProjects[ i ]->Save( GetString( SAVEDIR_DEVPR ) );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _devProjects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < _projects.size(); i++ )
	{
		NrpText saveFile = _projects[ i ]->Save( GetString( SAVEDIR_PROJECTS ) );
		sv.Set( SECTION_DEVPROJECTS, CreateKeyType(i), _projects[ i ]->ObjectTypeName() );
		sv.Set( SECTION_PROJECTS, CreateKeyProject(i), saveFile );
	}

	for( u32 i=0; i < users_.size(); i++ )
	{
		users_[ i ]->Save( GetString( SAVEDIR_USERS ) );
		sv.Set( SECTION_USERS, CreateKeyType(i),users_[ i ]->ObjectTypeName() );
		sv.Set( SECTION_USERS, CreateKeyUser(i), users_[ i ]->GetString( NAME ) );
	}

	for( u32 i=0; i < technologies_.size(); i++ )
	{
		NrpText saveFile = technologies_[ i ]->Save( GetString( SAVEDIR_TECHS ) );
		sv.Set( SECTION_TECHS, CreateKeyTech(i), saveFile );
	}

	for( u32 i=0; i < inventions_.size(); i++ )
	{
		NrpText inventSaveFile = inventions_[ i ]->Save( GetString( SAVEDIR_INVENTIONS ) + inventions_[ i ]->GetString( COMPANYNAME ), true );
		sv.Set( SECTION_INVENTIONS, CreateKeyInvention( i ), inventSaveFile );
	}

	for( u32 i=0; i < engines_.size(); i++ )
	{
		NrpText saveFolder = engines_[ i ]->Save( GetString( SAVEDIR_ENGINES ) );
		sv.Set( SECTION_ENGINES, CreateKeyEngine(i), saveFolder );
	}

	for( u32 i=0; i < games_.size(); i++ )
	{
		NrpText saveDir = games_[ i ]->Save( GetString( SAVEDIR_GAMES ) );
		sv.Set( SECTION_GAMES, CreateKeyGame( i ), saveDir );
	}
}

void CNrpApplication::_LoadUsers( const NrpText& fileName )
{
	IniFile sv( fileName );
	int maxUser = GetValue<int>( USERNUMBER );
	for( int i=0; i < maxUser; i++ )
	{
		NrpText name = sv.Get( SECTION_USERS, CreateKeyUser(i), NrpText("") );
		NrpText className = sv.Get( SECTION_USERS, CreateKeyType(i), NrpText("") );

		NrpText fileName = GetString( SAVEDIR_USERS ) + name + ".user";
		
		IUser* user = NULL;
		if( className == CNrpPlayer::ClassName() ) 
			user = new CNrpPlayer( fileName );
		else
		{
			if( className == CNrpAiUser::ClassName() )
				user = new CNrpAiUser( fileName );
			else
			{
				user = new IUser( className, "" );
				user->Load( fileName );
			}
		}

		AddUser( user );
	}
}

void CNrpApplication::_InitialyzeSaveDirectories( const NrpText& profileName )
{
	NrpText profileDir = OpFileSystem::CheckEndSlash( GetString( SAVEDIR ) + profileName );
	SetString( SAVEDIR_PROFILE, profileDir );
	SetString( SAVEDIR_INVENTIONS, profileDir + "inventions/" );
	SetString( SAVEDIR_COMPANIES, profileDir + "companies/" );
	SetString( SAVEDIR_DEVPR, profileDir + "devProjects/" );
	SetString( SAVEDIR_GAMES, profileDir + "games/");
	SetString( SAVEDIR_ENGINES, profileDir + "engines/" );
	SetString( SAVEDIR_PROJECTS, profileDir + "projects/" );
	SetString( SAVEDIR_PLANT, profileDir + "plant/" );
	SetString( SAVEDIR_USERS, profileDir + "users/" );
	SetString( SAVEINI_PROFILE, profileDir + "profile.ini" );
	SetString( SAVEDIR_TECHS, profileDir + "techs/" );
}

void CNrpApplication::Load( const NrpText& profileName, const NrpText& companyName )
{
	_InitialyzeSaveDirectories( profileName );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_CREATE );

	NrpText profileIni = GetString( SAVEINI_PROFILE );
	INrpConfig::Load( profileIni );

	_LoadUsers( profileIni );	

	IniFile rv( profileIni );

	for( int i=0; i < GetValue<int>( TECHNUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_TECHS, CreateKeyTech(i), NrpText("") );
		CNrpTechnology* tech = new CNrpTechnology( fileName ); //loading
		technologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( ENGINES_NUMBER ); i++ )
	{
		NrpText saveFolder = rv.Get( SECTION_ENGINES, CreateKeyEngine(i), NrpText("") );
		engines_.push_back( new CNrpGameEngine( saveFolder, true ) );
	}

	for( int i=0; i < GetValue<int>( DEVELOPPROJECTS_NUMBER ); i++ )
	{
		NrpText type = rv.Get( SECTION_DEVPROJECTS, CreateKeyType( i ), NrpText("") );
		NrpText name = rv.Get( SECTION_DEVPROJECTS, CreateKeyProject( i ), NrpText("") );
		if( type == CNrpDevelopGame::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( GetString( SAVEDIR_DEVPR ) + name );
			_devProjects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < GetValue<int>( PROJECTNUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_PROJECTS, CreateKeyProject( i ), NrpText("") );
		NrpText type = rv.Get( SECTION_PROJECTS, CreateKeyType( i ), NrpText("") );
		if( type == CNrpGameProject::ClassName() )
		{
			CNrpDevelopGame* game = new CNrpDevelopGame( GetString( SAVEDIR_PROJECTS ) + fileName );
			_projects.push_back( game );
		}
		else if( type == NrpText("devengine") )
		{
			//
		}
	}

	for( int i=0; i < GetValue<int>( GAMENUMBER ); i++ )
	{
		NrpText fileName = rv.Get( SECTION_GAMES,  CreateKeyGame( i ), NrpText("") );
		PNrpGame game = new CNrpGame( fileName );
		games_.push_back( game );
	}

	for( int k=0; k < GetValue<int>( INVENTIONSNUMBER ); k++ )
	{
		NrpText saveFile = rv.Get( SECTION_INVENTIONS, CreateKeyInvention( k ), NrpText("") );
		CNrpInvention* invention = new CNrpInvention( saveFile );
		inventions_.push_back( invention );
	}
	SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

	for( int k=0; k < GetValue<int>( COMPANIESNUMBER ); k++ )
	{
		NrpText fileName = rv.Get( SECTION_COMPANIES, CreateKeyCompany( k ), NrpText("") );
		CNrpCompany* cmp = new CNrpCompany( GetString( SAVEDIR_COMPANIES ) + fileName );
		AddCompany( cmp );
	}
	SetValue<int>( GAMENUMBER, games_.size() );

	CNrpScript::Instance().TemporaryScript( AFTER_LOAD_SCRIPT, CNrpScript::SA_EXEC );
}

void CNrpApplication::LoadScreenshots( const NrpText& fileName )
{
	IniFile rv( fileName );
	size_t imageListNumber = rv.Get( SECTION_OPTIONS, "screenshotNumber", (int)0 );

	if( !OpFileSystem::IsExist( fileName ) )
		return;
	
	for( size_t i=0; i < imageListNumber; i++ )
	{
		NrpText scrFile = rv.Get( SECTION_OPTIONS, CreateKeyScreenshot( i ), NrpText("") );
		_screenshots.push_back( new CNrpScreenshot( scrFile ) );
	}
}

void CNrpApplication::CreateProfile( const NrpText& profileName, const NrpText& companyName )
{
	SetString( PROFILENAME, profileName );
	SetString( PROFILECOMPANY, companyName );

	_InitialyzeSaveDirectories( profileName );
}

template< class T > 
void ClearArray( T& arrayt )
{
	for( u32 i=0; i < arrayt.size(); i++ )
		delete arrayt[ i ];
	arrayt.clear();
}

void CNrpApplication::ResetData()
{
	ClearArray( _companies );
	ClearArray( users_ );
	ClearArray( technologies_ );
}

CNrpGameEngine* CNrpApplication::GetGameEngine( const NrpText& name ) 
{
	return FindByNameAndIntName<ENGINES, CNrpGameEngine>( engines_, name );
}

void CNrpApplication::_BeginNewDay()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewDay( GetValue<SYSTEMTIME>( CURRENTTIME ) );

	CNrpPlant::Instance().BeginNewDay();
	UpdateMarketGames_();
	UpdateInvention_();
}

void CNrpApplication::UpdateInvention_()
{
	for( u32 k=0; k < inventions_.size(); k++ )
	{
		CNrpInvention* pInv = inventions_[ k ];
		pInv->CheckParams();

		if( pInv->GetValue<float>( READYWORKPERCENT ) >= 1 )
		{
			pInv->SetValue<SYSTEMTIME>( ENDDATE, GetValue<SYSTEMTIME>( CURRENTTIME ) );
			InventionFinished( pInv );
			k--;
		}
	}
}

float GetConsumerAbility_( float price )
{
	if( price > 150 ) return 10 / price;

	if( price > 100 && price <= 150 ) return 20 / price;

	if( price > 50 && price <= 100) return 40 / price;

	if( price > 25 && price <= 50 ) return 50 / price;

	if( price > 10 && price <= 25) return 25 / price;
	else return 30 / price;
}

int CNrpApplication::GetFreePlatformNumberForGame_( CNrpGame* game )
{
	int yearRaznost = GetValue<SYSTEMTIME>( CURRENTTIME ).wYear - 1980;

	int summ = 5000;
	for( int k=0; k < yearRaznost; k++ )
		 summ += summ * yearRaznost;

	return summ * game->GetValue<int>( PLATFORMNUMBER );
}

int CNrpApplication::GetSalesNumber_( CNrpGame* game )
{
	CNrpCompany* cmp = game->GetValue<PNrpCompany>( PARENTCOMPANY );
	assert( cmp );
	if( !cmp )
		return 0;

	//������� ���������� �������� �� ������� ����� ���� ������� ����
	int freePlatformNumber = GetFreePlatformNumberForGame_( game );
	
	//������ ���������� ��� ����� �����
	float gamesInThisGenre = 1;
	for( u32 i=0; i < games_.size(); i++ )
	{
		CNrpGame* tmpGame = games_[ i ];
		if( (game != tmpGame) && 
			game->GetValue<bool>( GAMEISSALING ) &&
			(tmpGame->GetGenreName( 0 ) == game->GetGenreName( 0 )) )
		  gamesInThisGenre += game->GetValue<int>( CURRENTGAMERATING ) / 100.f; 
	}

	freePlatformNumber -= game->GetValue<int>( COPYSELL );
	float userModificator = 1, compannyFamous = 1;
	if( cmp )
	{
		userModificator = cmp->GetUserModificatorForGame( game );
		compannyFamous = cmp->GetValue< float>( FAMOUS ); 
	}

	float authorFamous = 1;
	authorFamous = game->GetAuthorFamous();
	NrpText retailerName = game->GetString( GAMERETAILER );
	PNrpRetailer retailer = GetRetailer( retailerName );

	float retailerFamous = 0.1f;
	if( retailer )
		retailerFamous = retailer->GetValue<float>( FAMOUS );

	float genreInterest = GetGameGenreInterest( game );

	//������� ��� ����� ���� ������� �������
	freePlatformNumber /= 365;
	//����������� ��� ���������� ������� �������� �� ���� ���� ����� ��������� ������ � ������
	int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

	//��������� ������ ���� �� ���� ������� ����, ����������� ������� � ���������� �������������
	gameMaySaledToday *= ( game->GetValue<float>( FAMOUS ) + userModificator + authorFamous );

	//����������� ������ �� ����������� ��������� � ��������
	gameMaySaledToday *= (compannyFamous + retailerFamous) * 0.5f;

	//����������� �������������� �����������
	if( game->GetValue<PNrpGameBox>( GBOX ) )
		gameMaySaledToday *= GetConsumerAbility_( static_cast<float>( game->GetValue<PNrpGameBox>( GBOX )->GetValue<int>( PRICE ) ) );

	return gameMaySaledToday;
}

void CNrpApplication::UpdateMarketGames_()
{
	for( u32 i=0; i < games_.size(); i++ )
	{
		CNrpGame* rGame = games_[ i ];
		if( !rGame->GetValue<bool>( GAMEISSALING ) )
			continue;

		rGame->GameBoxSaling( GetSalesNumber_( rGame ) );
	}
}

void CNrpApplication::CreateNewFreeUsers()
{
	USERS coders, designer, composer, tester;
	core::map< NrpText, USERS* > group;
	group[ "coder" ] = &coders;
	group[ "designer" ] = &designer;
	group[ "composer" ] = &composer;
	group[ "tester" ] = &tester;
	
	for( u32 i=0; i < users_.size(); i++ )
	{
		IUser* user = users_[ i ];
		NrpText typeName = user->ObjectTypeName();
		if( user->GetValue<PNrpCompany>( PARENTCOMPANY ) != NULL )
			continue;

		if( core::map< NrpText, USERS* >::Node* node = group.find( typeName ) )
			node->getValue()->push_back( user );
	}

	size_t USER_GROUP_COUNT = 6;

	core::map< NrpText, USERS* >::Iterator gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
	{
		USERS& tmpList = *(gIter->getValue());
		for( u32 k=0; k < tmpList.size(); k++ )
			RemoveUser( tmpList[ k ] );

		tmpList.clear();

		for( size_t cnt=tmpList.size(); cnt < USER_GROUP_COUNT; cnt++ )
			tmpList.push_back( CreateRandomUser_( gIter->getKey() ) );
	}

	gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
		for( u32 cnt=0; cnt < gIter->getValue()->size(); cnt++ )
			users_.push_back( (*gIter->getValue())[ cnt ] );

	if( GetValue<int>( USERNUMBER ) != users_.size() )
		DoLuaFunctionsByType<void>( APP_USER_MARKETUPDATE, NULL );

	SetValue<int>( USERNUMBER, users_.size() );
}

CNrpApplication& nrp::CNrpApplication::Instance()
{
	if( !globalApplication )
		globalApplication = new CNrpApplication();

	return *globalApplication;
}

template< class B > int GetQuality_( B* ptrObject )
{
	if( ptrObject )
		return ptrObject->GetValue<int>( QUALITY );
	return 0;
}

void CNrpApplication::_UpdateGameRating( CNrpGame* ptrGame, GAME_RATING_TYPE typeRating )
{
	int rating = 0;

	CNrpCompany* cmp = ptrGame->GetValue<PNrpCompany>( PARENTCOMPANY );
	if( cmp != NULL )
	{		
		int number = 0;

		rating = GetQuality_( cmp->GetGameEngine( ptrGame->GetValue<NrpText>( GAME_ENGINE ) ) );

		for( int k=0; k < ptrGame->GetValue<int>( MODULE_NUMBER ); k++ )
		{
			NrpText name = ptrGame->GetTechName( k );
			rating += GetQuality_( GetTechnology( name ) );
			rating /= 2;
		}

		//��������� ������� ������� �� ����� ����
		int monthInMarket = TimeHelper::GetMonthBetweenDate( ptrGame->GetValue<SYSTEMTIME>( STARTDATE ),
															 GetValue<SYSTEMTIME>( CURRENTTIME ) ) + 1;
		//�������� ������� ��-�� ������� �� �����
		rating *= 1.f / (monthInMarket > 12 ? 12 :  monthInMarket);

		//��������� �������� ��������
		//todo: ���� ����� �������� ������� ������� ���
		ptrGame->GetHistory()->AddStep( GetValue<SYSTEMTIME>( CURRENTTIME ) )->AddValue<int>( CURRENTGAMERATING, rating );
	}
}

void CNrpApplication::UpdateGameRatings( CNrpGame* ptrGame, bool firstTime )
{
		_UpdateGameRating( ptrGame, GRT_GENERAL );
	/*	ptrGame->SetValue<int>( STARTGRAPHICRATING, GetGameRating_( ptrGame, GRT_VIDEO ) );
		ptrGame->SetValue<int>( STARTGENRERATING, GetGameRating_( ptrGame, GRT_GENRE ) );
		ptrGame->SetValue<int>( STARTSOUNDRATING, GetGameRating_( ptrGame, GRT_SOUND ) );
		ptrGame->SetValue<int>( STARTADVFUNCRATING, GetGameRating_( ptrGame, GRT_ADVFUNC ) ); */
}

IUser* CNrpApplication::CreateRandomUser_( NrpText userType )
{
	size_t randomParams = 1 + rand() % (GT_COUNT % 100);//������� ���������� ����� ���������
	size_t maxParamValue = 1 + rand() % 100;//������������ �������� ����������

	std::map< NrpText, int > skillMap;
	skillMap[ "coder" ] = SKL_CODING;
	skillMap[ "designer" ] = SKL_DRAWING;
	skillMap[ "composer" ] = SKL_SOUND;
	skillMap[ "tester" ] = SKL_TESTING;

	NrpText userName;

	IUser* ptrUser = NULL;
	do 
	{
		userName = GlobalPeopleName[ rand() % PEOPLE_NAME_COUNT ] + " " + GlobalPeopleSurname[ rand() % PEOPLE_SURNAME_COUNT ];
		ptrUser = GetUser( userName );
	} while ( ptrUser != NULL );

	ptrUser = new IUser( userType, userName );
	ptrUser->SetSkill( skillMap[ userType ], maxParamValue );
	ptrUser->SetValue<int>( CODE_QUALITY, rand() % maxParamValue );
	ptrUser->SetValue<int>( CODE_SPEED, rand() % maxParamValue );
	ptrUser->SetValue<int>( TALANT, rand() % maxParamValue );
	ptrUser->SetValue<int>( STAMINA, rand() % maxParamValue );
	ptrUser->SetValue<int>( STABILITY, rand() % maxParamValue );
	ptrUser->SetValue<int>( CHARACTER, rand() % maxParamValue );

	if( ptrUser->GetValue<int>( WANTMONEY ) < GetValue<int>( MINIMUM_USER_SALARY ) )
		ptrUser->SetValue<int>( WANTMONEY, GetValue<int>( MINIMUM_USER_SALARY ) );

	char name[64] = { 0 };
	snprintf( name, 64, "media/face/face%03d.png", rand() % 2 );
	ptrUser->SetString( TEXTURENORMAL, name );

	for( size_t cnt=0; cnt < randomParams; cnt++ )
	{
		ptrUser->SetGenreExperience( rand() % (GT_COUNT%100), rand() % maxParamValue );
		ptrUser->SetGenrePreferences( rand() % (GT_COUNT%100), rand() % maxParamValue );
	} 

	return ptrUser;
}

void CNrpApplication::_BeginNewMonth()
{
	//������ ������ � ��������
	for( u32 i=0; i < _companies.size(); i++)
		_companies[ i ]->BeginNewMonth( GetValue<SYSTEMTIME>( CURRENTTIME ));

	//��������� �������� ���
	for( u32 i=0; i < games_.size(); i++ )
		 if( games_[ i ]->GetValue<bool>( GAMEISSALING ) )
			 UpdateGameRatings( games_[ i ] );
}

void CNrpApplication::_BeginNewHour()
{
	for( u32 i=0; i < _companies.size(); i++)
		 _companies[ i ]->BeginNewHour( GetValue<SYSTEMTIME>( CURRENTTIME ));
}

bool CNrpApplication::AddBoxAddon( CNrpTechnology* tech )
{
	if( GetBoxAddon( tech->GetString( NAME ) ) == NULL  )
	{
		boxAddons_.push_back( tech );
		SetValue<int>( BOXADDONNUMBER, boxAddons_.size() );
		return true;
	}

	return false;
}

CNrpTechnology* CNrpApplication::GetBoxAddon( const NrpText& name )
{
	return FindByNameAndIntName< TECHS, CNrpTechnology >( boxAddons_, name );
}

void CNrpApplication::AddGameToMarket( CNrpGame* game )
{
	assert( game != NULL );
	if( !game || game->GetValue<bool>( GAMEISSALING ) )
		return;

	game->SetValue<bool>( GAMEISSALING, true );

	//����� ���� ������� �� �����, �� ��� ������ �� ����
	for( int i=0; i < game->GetValue<int>( GENRE_MODULE_NUMBER ); i++ )
	{
		NrpText genreName = game->GetGenreName( i );
		//������� �������� � ��������� �������� � ����� ����
		CNrpTechnology* tech = GetTechnology( genreName );
		if( tech != NULL )
			tech->AddValue<float>( INTEREST, -game->GetValue<int>( STARTGAMERATING ) / 1000.f );
	}

	games_.push_back( game );
	SetValue( GAMENUMBER, games_.size() );
}

//������� � ����� �������� � ��������������� ������� �� 10% �� �������� ����
float CNrpApplication::GetGameGenreInterest( CNrpGame* game )
{
	CNrpTechnology* tech = GetTechnology( game->GetGenreName( 0 ) );
	float summ = tech != NULL ? tech->GetValue<float>( INTEREST ) : 0.1f;
	int gm = game->GetValue<int>( GENRE_MODULE_NUMBER );
	for( int i=1; i < gm; i++ )
	{
		tech = GetTechnology( game->GetGenreName( i ) );	
		summ += ( tech != NULL ? (tech->GetValue<float>( INTEREST ) / i) : 0 );
	}

	//���� ��������������� ��������� �������� � �����, ������� ��������
	//���� ��� ���������� �� �����
	return (summ + game->GetValue<int>( CURRENTGAMERATING ) / 1000.f );
}

CNrpRetailer* CNrpApplication::GetRetailer( const NrpText& name )
{
	return NULL;
}

//��������� ����� ����������� � ������� ����� ������ ������� ����
NrpText CNrpApplication::GetFreeInternalName( CNrpGame* game )
{
	SCREENSHOTS	thisYearAndGenreImgs;
	
	int minimumRating = 1;
	for( u32 i=0; i < _screenshots.size(); i++ )
	{
		CNrpGame* alsoMargetGame = GetGame( _screenshots[ i ]->GetString( NAME ) );

		if( !alsoMargetGame ) 
		{
			int year = GetValue<SYSTEMTIME>( CURRENTTIME ).wYear;
			if( CNrpGameEngine* ge = GetGameEngine( game->GetString( GAME_ENGINE) ) )
				year = ge->GetValue<SYSTEMTIME>( STARTDATE ).wYear;

			if( !_screenshots[ i ]->IsMyYear( year ) )
				continue;
			
			int eqRating = _screenshots[ i ]->GetEqualeRating( game );

			if( !eqRating )
				continue;

			if( minimumRating > eqRating )
				thisYearAndGenreImgs.clear();

			minimumRating = eqRating;
			thisYearAndGenreImgs.push_back( _screenshots[ i ] );
		}
	}

	if( thisYearAndGenreImgs.size() )
	{
		int randomIndex = rand() % thisYearAndGenreImgs.size();
		return thisYearAndGenreImgs[ randomIndex ]->GetString( NAME );
	}

	//!!!!!!!!���� ����� ���������� ��� ��������!!!!!!!!!!!!!!!!!
	assert( NrpText("No find free name").size() == 0 );
	return "";
}

CNrpScreenshot* CNrpApplication::GetScreenshot( const NrpText& name )
{
	return FindByName< SCREENSHOTS, CNrpScreenshot >( _screenshots, name );
}

CNrpGame* CNrpApplication::GetGame( const NrpText& name )
{
	return FindByNameAndIntName< GAMES, CNrpGame >( games_, name );
}

CNrpGame* CNrpApplication::GetGame( u32 index )
{
	assert( index < games_.size() );
	return index < games_.size() ? games_[ index ] : NULL;
}

void CNrpApplication::RemoveTechnology( CNrpTechnology* ptrTech )
{
	for( u32 i=0; i < technologies_.size(); i++ )
		if( technologies_[ i ] == ptrTech )
		{
			technologies_.erase( i );
			return;
		}

	Log(HW) << "unknown technology" << term;
}

void CNrpApplication::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
	if( GetGameEngine( ptrEngine->GetString( INTERNAL_NAME ) ) == NULL )
	{
		engines_.push_back( ptrEngine );
		SetValue<int>( ENGINES_NUMBER, engines_.size() );
		return;
	}

	Log( HW ) << "����� ������ ��� ����" << term;
}

INrpProject* CNrpApplication::GetProject( const NrpText& name )
{
	return FindByName< PROJECTS, INrpProject >( _projects, name );
}

void CNrpApplication::AddDevelopProject( nrp::INrpDevelopProject* project )
{
	if( FindByName< DEVPROJECTS, INrpDevelopProject >( _devProjects, project->GetString( NAME ) ) )
	{
		_devProjects.push_back( project );
		SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
	}
}

INrpDevelopProject* CNrpApplication::GetDevelopProject( const NrpText& name )
{
	return FindByName<DEVPROJECTS, INrpDevelopProject>( _devProjects, name );
}

void CNrpApplication::RemoveDevelopProject( const NrpText& name )
{
	for( u32 i=0; i < _devProjects.size(); i++ )
	{
		if( _devProjects[ i ]->Equale( name ) )
		{
			delete _devProjects[ i ];
			_devProjects.erase( i );
			break;
		}
	}

	SetValue<int>( DEVELOPPROJECTS_NUMBER, _devProjects.size() );
}

void CNrpApplication::AddGame( CNrpGame* ptrGame )
{
	assert( ptrGame != NULL );
	games_.push_back( ptrGame );
	SetValue<int>( GAMENUMBER, games_.size() );
}

void CNrpApplication::AddInvention( const NrpText& name, CNrpCompany* parentCompany )
{
	CNrpInvention* tmp = GetInvention( name, parentCompany->GetValue<NrpText>( NAME ) );

	if( tmp == NULL )
	{
		CNrpTechnology* startTech = new CNrpTechnology( NrpText( "xtras/technology/" ) + name + "/item.tech" );

		CNrpInvention* inv = new CNrpInvention( startTech, parentCompany );
		parentCompany->AddInvention( inv );	
		inventions_.push_back( inv );  
		SetValue<int>( INVENTIONSNUMBER, inventions_.size() );

		delete startTech;
	}
}

void CNrpApplication::InventionFinished( CNrpInvention* ptrInvention )
{
	//������� ��������������� ����������� ���������� 
	//���������� � � ������ ���������
	CNrpTechnology* tech = new CNrpTechnology( ptrInvention );
	AddTechnology( tech );

	int delPos = -1;
	for( u32 i=0; i < inventions_.size(); i++ )
	{
		CNrpInvention* inv = inventions_[ i ];
		if( inv->GetString( NAME ) == ptrInvention->GetString( NAME ) )
		{
			PNrpCompany pCmp = inv->GetValue<PNrpCompany>( PARENTCOMPANY );
			if( pCmp == ptrInvention->GetValue<PNrpCompany>( PARENTCOMPANY) )
			{
				delPos = i;//����� ��� ����������� � ����� ������ � ������� ��� ������...
				pCmp->AddTechnology( tech );
				pCmp->RemoveInvention( ptrInvention );
				continue;
			}
		
			pCmp->InventionReleased( ptrInvention );//��������� ��� �������� �� ����������� ����������
		}
	}

	DoLuaFunctionsByType( APP_INVENTION_FINISHED, tech );
	
	delete inventions_[ delPos ];
	inventions_.erase( delPos );
}

void CNrpApplication::InventionCanceled( CNrpInvention* ptrInvention )
{
	for( u32 i=0; i < inventions_.size(); i++ )
	{
		CNrpInvention* inv = inventions_[ i ];
		if( inv == ptrInvention )
		{
#ifdef _DEBUG
			NrpText text = "������� ����������";
			text += inv->GetString( NAME );
			PNrpCompany cmp = inv->GetValue<PNrpCompany>( PARENTCOMPANY );
			assert( cmp != NULL );
			text += cmp ? cmp->GetString( NAME ) : NrpText("unknown company");
			Log( HW ) << text << term;
#endif
			delete inv;
			inventions_.erase( i );
			break;
		}
	}
}

CNrpInvention* CNrpApplication::GetInvention( const NrpText& name, const NrpText& companyName )
{
	for( u32 i=0; i < inventions_.size(); i++ )
		if( inventions_[ i ]->Equale( name, companyName ) )
			return inventions_[ i ];

	return NULL;
}

NrpText CNrpApplication::ClassName()
{
	return CLASS_NRPAPPLICATION;
}
}//namespace nrp