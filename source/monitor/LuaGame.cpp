#include "StdAfx.h"
#include <assert.h>

#include "LuaGame.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpExtInfo.h"
#include "LuaTechnology.h"
#include "LuaCompany.h"
#include "NrpHistory.h"
#include "NrpTime.h"
#include "NrpApplication.h"
#include "HTMLEngine.h"
#include "NrpGameMarket.h"
#include "NrpCompany.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAGAME( "CLuaGame" );

const NrpText CLuaGame::defaultDescSave = "media/html/desc.htm";
const NrpText CLuaGame::defaultDescTemplate = "media/html/fish.htm";

BEGIN_LUNA_METHODS(CLuaGame)
	LUNA_AUTONAME_FUNCTION( CLuaGame, IsMyBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaGame, AddBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaGame, CreateBox )
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBox )
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxAddon )
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxImage )
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetScreenshot )
	LUNA_AUTONAME_FUNCTION( CLuaGame, Create )
    LUNA_AUTONAME_FUNCTION( CLuaGame, CreateHistory )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGame)
	LUNA_ILUABASEPROJECT_PROPERTIES( CLuaGame )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "boxAddonsNumber", GetBoxAddonsNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "boxImageNumber", GetBoxImageNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "screenshotNumber", GetScreenshotNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "inSale", IsSaling, PureFunction )
    LUNA_AUTONAME_PROPERTY( CLuaGame, "npc", GetNpcGame, SetNpcGame )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "name", GetName, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "haveBox", HaveBox, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "boxLevel", GetBoxLevel, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "viewImage", GetViewImage, SetViewImage )
    LUNA_AUTONAME_PROPERTY( CLuaGame, "companyName", GetCompanyName, SetCompanyName )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "company", GetCompany, SetCompany )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "lastMonthSales", GetLastMonthSales, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "price", GetPrice, SetPrice )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "lastMonthProfit", GetLastMonthProfit, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "allTimeSales", GetAllTimeSales, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "allTimeProfit", GetAllTimeProfit, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "description", GetDescriptionLink, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "rating", GetRating, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "graphikRating", GetGraphikRating, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "soundRating", GetSoundRating, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "gameplayRating", GetGameplayRating, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "bugsRating", GetBugsRating, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaGame, "randomRecense", GetRandomRecense, PureFunction )
END_LUNA_PROPERTIES

CLuaGame::CLuaGame(lua_State *L, bool ex) : ILuaBaseProject(L, CLASS_LUAGAME, ex )							//�����������
{}

int CLuaGame::HaveBox( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpGameBox>( L, PROP, GBOX, NULL ) != NULL );
	return 1;	
}

int CLuaGame::SetCompany( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN
    {
        CNrpCompany* cmp = _GetLuaObject< CNrpCompany, ILuaObject >( L, -1, true );
        (*_object)[ PARENTCOMPANY ] = cmp;
    }

    return 0;
}

int CLuaGame::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, NAME, "undeclared param" ) );
	return 1;	
}

int CLuaGame::IsMyBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:IsMyBoxAddon need string parameter" );

	NrpText name = lua_tostring( L, 2 );

	bool isMyBox = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		if( box != NULL )
			isMyBox = box->IsMyAddon( name );

	}
	lua_pushboolean( L, isMyBox );
	return 1;	
}

int CLuaGame::RemoveBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:RemoveBoxAddon need string parameter" );	

	IF_OBJECT_NOT_NULL_THEN
	{
		if( lua_islightuserdata( L, 2 ) )
		{
			CNrpTechnology* tech = _GetLuaObject< CNrpTechnology, ILuaObject >( L, 2, true );
			PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
			assert( box && tech );
			if( box && tech )
				box->RemoveAddon( *tech );
		}
		else if( lua_isstring( L, 2 ) )
		{
			NrpText name = lua_tostring( L, 2 );
			PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
			assert( box );
			if( box != NULL )
				box->RemoveAddon( name );
		}
	}
	return 1;	
}

int CLuaGame::AddBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:AddBoxAddon need CNrpTechnology parameter" );

	CNrpBoxAddon* addon = _GetLuaObject< CNrpBoxAddon, ILuaObject >( L, 2, true );
	assert( addon != NULL );

	bool ret=false;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		assert( box != NULL );
		if( box != NULL )
			ret = box->AddAddon( addon );
	}

	lua_pushboolean( L, ret );
	return 1;	
}

int CLuaGame::GetBoxAddonsNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		if( box != NULL)
		{
			lua_pushinteger( L, (*box)[ NUMBERADDON ] );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::CreateBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:CreateBox need int parameter" );

	int boxLevel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = new CNrpGameBox( _object );
		(*box)[ LEVEL ] = boxLevel;
		(*_object)[ GBOX ] = box ;
	}
	return 1;		
}

int CLuaGame::RemoveBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:RemoveBox not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		(*_object)[ GBOX ] = (PNrpGameBox)NULL;
		delete box;
	}

	return 1;			
}

int CLuaGame::GetBoxLevel( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		if( box != NULL )
		{
			int boxLevel = (*box)[ LEVEL ];
			lua_pushinteger( L, boxLevel );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;			
}

int CLuaGame::GetBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:GetBoxAddon need int parameter" );

	int index = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		if( box )
        {
			CNrpTechnology* tech = box->GetAddon( index );
            lua_pushlightuserdata( L, tech );
            Luna< CLuaTechnology >::constructor( L );
            return 1;
        }
	}

    lua_pushnil( L );
	return 1;		
}

int CLuaGame::GetNpcGame( lua_State* L )
{
    lua_pushboolean( L, GetParam_<bool>( L, PROP, NPC_GAME, true ) );
    return 1;		
}

int CLuaGame::IsSaling( lua_State* L )
{
	lua_pushboolean( L, GetParam_<bool>( L, PROP, GAMEISSALING, true ) );
	return 1;		
}

int CLuaGame::GetImagePath_( lua_State* L, const NrpText& funcName, OPTION_NAME& nameParam )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + " need index parameter") );

	int index = lua_tointeger( L, 2 );
	NrpText pathTexture;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpExtInfo* imageList = (*_object)[ EXTINFO ].As<CNrpExtInfo*>();
		if( imageList )
		{
			int maxImage = (*imageList)[ nameParam ];
			if( maxImage > 0)
			{
				const STRINGS& pvm = (nameParam == IMAGESBOXNUMBER) 
															? imageList->GetBoxImages() 
															: imageList->GetImages();
				pathTexture = pvm[ index < 0 ? (rand() % maxImage) : index ];
			}
		}
	}

	lua_pushstring( L, pathTexture );
	return 1;		
}

int CLuaGame::GetBoxImage( lua_State* L )
{
	return GetImagePath_( L, "GetBoxImage", IMAGESBOXNUMBER );
}

template< class R > R CLuaGame::GetImageLisParam_( lua_State* L, const NrpText& funcName, OPTION_NAME& name, R defValue )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpExtInfo* imageList = (*_object)[ EXTINFO ].As<CNrpExtInfo*>();
		if( imageList )
			defValue = (*imageList)[ name ];
	}

	return defValue;
}

int CLuaGame::GetBoxImageNumber( lua_State* L )
{
	lua_pushinteger( L, GetImageLisParam_<int>( L, PROP, IMAGESBOXNUMBER, 0 ) );
	return 1;		
}

int CLuaGame::GetScreenshotNumber( lua_State* L )
{
	lua_pushinteger( L, GetImageLisParam_<int>( L, PROP, IMAGESNUMBER, 0 ) ); 
	return 1;
}

int CLuaGame::GetScreenshot( lua_State* L )
{
	return GetImagePath_( L, "GetScreenShot", IMAGESNUMBER );
}

int CLuaGame::SetViewImage( lua_State* L )
{
	return SetParam_( L, PROP, VIEWIMAGE );
}

int CLuaGame::GetViewImage( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, VIEWIMAGE, "undeclared param" ) );
	return 1;	
}

int CLuaGame::GetLastMonthProfit( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int sales = 0;
		if( CNrpHistory* history = _object->GetHistory() )
		{
			if( CNrpHistoryStep* step = history->GetLast() )
            {
				lua_pushinteger( L, (*step)[ BALANCE ] );
		    	return 1;
            }
        }
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGame::GetLastMonthSales( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		if( CNrpHistory* history = _object->GetHistory() )
		{
			int sales = 0;
			if( CNrpHistoryStep* step = history->GetLast() )
				sales = (*step)[ BOXNUMBER ];

			lua_pushinteger( L, sales );
            return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

int CLuaGame::GetAllTimeSales( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
		if( CNrpHistory* history = _object->GetHistory() )
        {
			lua_pushinteger( L, history->GetSummFor( BOXNUMBER, _nrpApp[ CURRENTTIME ].As<NrpTime>() ) );
	  	    return 1;
        }

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::GetPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		assert( box != NULL );
		if( box != NULL )
		{
			int price = (*box)[ PRICE ];
			lua_pushinteger( L, price );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::SetNpcGame( lua_State* L )
{
    assert( lua_isboolean( L, -1 ) );
    IF_OBJECT_NOT_NULL_THEN (*_object)[ NPC_GAME ] = lua_toboolean( L, -1 ) != 0;
    return 0;		
}

int CLuaGame::SetPrice( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	int newPrice = lua_tointeger( L, -1 );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = (*_object)[ GBOX ].As<PNrpGameBox>();
		assert( box != NULL );
		if( box )
			(*box)[ PRICE ] = newPrice;
	}

	return 0;		
}

int CLuaGame::SetCompanyName( lua_State* L )
{
    assert( lua_isstring( L, -1 ) );
    IF_OBJECT_NOT_NULL_THEN 
    {
        NrpText cmpName = lua_tostring( L, -1 );
        CNrpCompany* cmp = _nrpApp.GetCompany( cmpName );
        assert( cmp );
        (*_object)[ PARENTCOMPANY ] = cmp;
    }

    return 0;		
}

int CLuaGame::GetCompanyName( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN 
    {
        NrpText cmpName = (*_object)[ OWNER ];
        lua_pushstring( L, cmpName );
        return 1;
    }

    lua_pushnil( L );
    return 1;		
}

int CLuaGame::GetCompany( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		if( CNrpCompany* cmp = (*_object)[ PARENTCOMPANY ].As<CNrpCompany*>() )
		{
			lua_pushlightuserdata( L, cmp );
			Luna< CLuaCompany >::constructor( L );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::CreateHistory( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:CreateHistory not need any parameter" );

    CNrpGame& game = *_object;
    int fMonth = NrpTime( game[ STARTDATE ] ).GetMonthToDate( _nrpApp[ CURRENTTIME ].As<NrpTime>() );//������ ���������� ������� ����� ����
    int price = ( rand() % 90 ) + 10;

    CNrpGameMarket& market = CNrpGameMarket::Instance();

    //����������� ������ ��� ������ ����
    //������� ����������� �� ������ � ����������� � ������ �������
    int plNumber = market[ PLATFORMNUMBER ];

    //�� ��� ��� �������� ���������� ���� + ���������� ����� ��� ��������
    int lastSale = market.GetPlatformSales( game, game[ STARTDATE ] ) / (int)game[ PLATFORMNUMBER ];

    game[ COPYSELL ] = 0;
    float step = 1.f / fMonth;
    for( int i=0; i < fMonth; i++ )
    {
        NrpTime ft( game[ STARTDATE ] );
        ft = ft.AppendMonth( i );

        //������� ������� ����� �������� ��� ���� ���� �������
        int numBox = market.GetPlatformSales( game, ft );

        //������ �� ������ ���������� ����� ����� ����
        numBox -= (int)game[ COPYSELL ];
        if( numBox < 0 )
            Log( HW ) << ft.ToText( "HHHH/MM/DD" ) << ":Negative sale(" << NrpText( numBox ) << ") for game " << (NrpText)game[ NAME ] << term;

        numBox = (std::max)( 0, numBox );
        if( numBox > 0 )
        {
            numBox /= (int)game[ PLATFORMNUMBER ];

            lastSale = market.CalcAmpChange( numBox, 20, lastSale * 0.9 );
            numBox = ( numBox + lastSale ) / 2;

            //�������� ���� ������ ����� ������
            price = CNrpGameMarket::CalcAmpChange( price, 10, 10 );
        }

        game.AddSales( numBox, ft, price );
    }

    return 0;
}

int CLuaGame::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:Create need string parameter" );

	NrpText fileName = lua_tostring( L, 2 );

	if( fileName != NULL )
	{
		_object = new CNrpGame( fileName  );
        CNrpCompany* cmp = _nrpApp.GetCompany( (NrpText)(*_object)[ OWNER ] );
        assert( cmp && "company must be exist");
        (*_object)[ PARENTCOMPANY ] = cmp;
		(*_object)[ NPC_GAME ] = true;
        cmp->AddGame( _object );

		lua_pushlightuserdata( L, _object );
		Luna< CLuaGame >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::GetDescriptionLink( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN
    {
        HTMLEngine::Instance().CreateDescription( CLuaGame::defaultDescTemplate, CLuaGame::defaultDescSave, *_object );

        lua_pushstring( L, const_cast< NrpText& >( CLuaGame::defaultDescSave ).ToStr() );
        return 1;	
    }

    lua_pushnil( L );
    return 1;
}

const char* CLuaGame::ClassName()
{
	return ( CLASS_LUAGAME );
}

int CLuaGame::GetAllTimeProfit( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		if( CNrpHistory* history = _object->GetHistory() )
			lua_pushinteger( L, history->GetSummFor( BALANCE, _nrpApp[ CURRENTTIME ].As<NrpTime>() ) );
		else
			lua_pushnil( L );

		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaGame::GetRandomRecense( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		if( CNrpExtInfo* imageList = (*_object)[ EXTINFO ].As<CNrpExtInfo*>() )
		{
			const STRINGS& recenses = imageList->GetRecenses();
			NrpText randRecense = "CLuaGame:GetRandomRecense:NoRecense"; 
			if( recenses.size() > 0 )
				randRecense = recenses[ rand() % recenses.size() ];

			lua_pushstring( L, randRecense );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;			
}

int CLuaGame::GetGraphikRating( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, CURRENTGRAPHICRATING, 0 ) );
	return 1;		
}

int CLuaGame::GetSoundRating( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, CURRENTSOUNDRATING, 0 ) );
	return 1;		
}

int CLuaGame::GetGameplayRating( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, CURRENTGENRERATING, 0 ) );
	return 1;		
}

int CLuaGame::GetBugsRating( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, CURRENTBUGRATING, 0 ) );
	return 1;		
}

int CLuaGame::GetRating( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, CURRENTGAMERATING, 0 ) );
	return 1;		
}

}//namespace nrp