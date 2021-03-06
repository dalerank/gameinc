#include "stdafx.h"
#include <lua.hpp>
#include <luna.h>
#include <irrlicht.h>

#include "nrpEngine.h"
#include "nrpScriptCore.h"
#include "nrpScript.h"
#include "LuaTerrain.h"
#include "LuaButton.h"
#include "LuaWindow.h"
#include "LuaDriver.h"
#include "LuaMiniMap.h"
#include "LuaCamera.h"
#include "LuaScrollBar.h"
#include "LuaSceneManager.h"
#include "LuaGuiEnvironment.h"
#include "LuaTable.h"
#include "LuaTexture.h"
#include "LuaConsole.h"
#include "LuaSceneNode.h"
#include "LuaElement.h"
#include "LuaEvent.h"
#include "LuaChart.h"
#include "LuaComboBox.h"
#include "LuaEdit.h"
#include "LuaImage.h"
#include "LuaLabel.h"
#include "LuaCompany.h"
#include "LuaApplication.h"
#include "LuaUser.h"
#include "LuaBank.h"
#include "LuaProgressBar.h"
#include "LuaProject.h"
#include "LuaLinkBox.h"
#include "LuaGameEngine.h"
#include "LuaGameProject.h"
#include "LuaTechnology.h"
#include "LuaListBox.h"
#include "LuaComponentListBox.h"
#include "LuaGameBox.h"
#include "LuaGame.h"
#include "LuaIniFile.h"
#include "LuaDiskMachine.h"
#include "LuaPlantWork.h"
#include "LuaPlant.h"
#include "LuaReklame.h"
#include "LuaPictureFlow.h"
#include "LuaDevelopModule.h"
#include "LuaDevelopProject.h"
#include "LuaTechMap.h"
#include "LuaBrowser.h"
#include "LuaInvention.h"
#include "LuaAutoScript.h"
#include "LuaRelation.h"
#include "LuaPda.h"
#include "LuaTab.h"
#include "LuaPlatform.h"
#include "LuaFlick.h"
#include "LuaAnimator.h"
#include "LuaSoundEngine.h"
#include "LuaQuest.h"
#include "LuaQuestEngine.h"
#include "LuaLink.h"

static nrp::CNrpScript* global_script_engine = NULL;

using irr::io::IWriteFile;
using irr::io::IFileSystem;

namespace nrp
{

CLASS_NAME CLASS_NRPSCRIPT( "CNrpScript" );
CNrpScript::CNrpScript() : INrpConfig( CLASS_NRPSCRIPT, CLASS_NRPSCRIPT ), vm_(0)
{
	RegProperty<NrpText>( LOAD_FUNCTIONS_FILENAME, "" );
	RegProperty<bool>( SHOW_CALL_FUNCTION_NAME, true );

	// NULL ���� ���� ������ ��������� ������
	if (!(vm_ = luaL_newstate()))
		Log(SCRIPT, FATAL) << "������������� �������������� NerpaScript �� ������� (������������ ������)" << term;

	// �� ���������
	luaL_openlibs(vm_);

	// ������������ ������� �� ���������� � http://www.lua.org/pil/	
	// �� ���� ������� �������� ���������� ��� �������� ������
	if (!luaL_newmetatable(vm_, "array"))
		Log(SCRIPT, FATAL) << "�������� ����������� ��� ��������� �������� ������ ��������." << term;
	
	try
	{
		// ������� ��� ��������� �������� � Lua
		// ����� ������ "m"
		lua_pushstring(vm_,"__tostring");
		lua_pushcfunction(vm_, tostring);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		// ����� ������ "m"
		lua_pushstring(vm_, "__index");
		lua_pushcfunction(vm_, GetArray);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		// ����� ������ "m"
		lua_pushstring(vm_ ,"__newindex");
		lua_pushcfunction(vm_, SetArray);
		// ����� ������ "e"
		lua_settable(vm_, -3);
		lua_register(vm_, "array", NewArray);


		// ������� ������� :-)
		lua_register(vm_, "NrpAboutRobots", AboutRobots);

		// ��������� �����
		lua_register(vm_, "NrpSocketOpen", SocketOpen);

		// ��������� �����
		lua_register(vm_, "NrpSocketClose", SocketClose);
		
		lua_register( vm_, "NrpCreateTerrainShader", CreateTerrainShader );

		lua_register( vm_, "NrpGetGuiEnvironment", GetGuiEnvironment );

		lua_register( vm_, "NrpGetApplication", GetApplication );
		// ������ � ������ �� Lua
		lua_register(vm_, "Log", ScriptLog );

		lua_register( vm_, "NrpGetVideoDriver", GetVideoDriver );				//��������� �������� ��� ������ � �����������

		lua_register( vm_, "NrpGetSceneManager", GetSceneManager );				//�������� ���� irrlicht

		lua_register( vm_, "NrpAddScene", AddScene );							//������������ ����� ��� ������

		lua_register( vm_, "NrpSetNextScene", SetNextScene );					//������������� ����� ��� ��������		

		lua_register( vm_, "NrpSetSceneOption", SetSceneOption );			//��������� ������� ��� ���������� ����� ������� �����

		lua_register( vm_, "NrpHelp", Help);										//???			

		lua_register( vm_, "NrpInitializeLoginScene", InitializeLoginScene );   //������� �����-����� 

		lua_register( vm_, "NrpInitializeWorkScene", InitializeWorkScene );		//������� �������-�����

		lua_register( vm_, "NrpLoadPlugins", LoadPlugins );

		lua_register( vm_, "NrpApplicationClose", ApplicationClose );

		lua_register( vm_, "NrpApplicationSave", ApplicationSave );

		lua_register( vm_, "NrpGetBrowser", GetBrowser );

		lua_register( vm_, "NrpGetTranslate", ApplicationGetTranslate );
		
		lua_register( vm_, "NrpDumpStack", ApplicationDumpStack );

		lua_register( vm_, "NrpARGB", ApplicationNrpRgb );

		RegisterLuaClasses_();
	}
	catch (...)
	{
		Log(SCRIPT, FATAL) << "����������� ��������� ������ � NrpScript ������ ��������." << term;
	}

	// �������� ���������� �� ��������� � ���� ������ ������� ��� Lua
	DoString("package.path = package.path..\";./scripts/?.lua\"");
}

void CNrpScript::RegisterLuaClasses_()
{
	Luna< CLuaWindow >::Register( vm_ );									//�������� ��� ����
	Luna< CLuaButton >::Register( vm_ );									//�������� ��� ������
	Luna< CLuaTerrain >::Register( vm_ );									//�����������		
	Luna< CLuaDriver >::Register( vm_ );									//�������
	Luna< CLuaMiniMap >::Register( vm_ );									//���������
	Luna< CLuaCamera >::Register( vm_ );									//������		
	Luna< CLuaScrollBar >::Register( vm_ );									//���������
	Luna< CLuaSceneManager >::Register( vm_ );								//���� ��������
	Luna< CLuaGuiEnvironment >::Register( vm_ );							//���-�������
	Luna< CLuaTable >::Register( vm_ );										//�������		
	Luna< CLuaTexture >::Register( vm_ );									//��������
	Luna< CLuaConsole >::Register( vm_ );
	Luna< CLuaSceneNode >::Register( vm_ );
	Luna< CLuaElement >::Register( vm_ );
	Luna< CLuaEvent >::Register( vm_ );
	Luna< CLuaChart >::Register( vm_ );
	Luna< CLuaComboBox >::Register( vm_ );
	Luna< CLuaEdit >::Register( vm_ );
	Luna< CLuaImage >::Register( vm_ );
	Luna< CLuaLabel >::Register( vm_ );
	Luna< CLuaCompany >::Register( vm_ );
	Luna< CLuaApplication >::Register( vm_ );
	Luna< CLuaUser >::Register( vm_ );
	Luna< CLuaBank >::Register( vm_ );
	Luna< CLuaBank >::Register( vm_ );
	Luna< CLuaProject >::Register( vm_ );
	Luna< CLuaProgressBar >::Register( vm_ );
	Luna< CLuaLinkBox >::Register( vm_ );
	Luna< CLuaGameEngine >::Register( vm_ );
	Luna< CLuaGameProject >::Register( vm_ );
	Luna< CLuaTechnology >::Register( vm_ );
	Luna< CLuaListBox >::Register( vm_ );
	Luna< CLuaComponentListBox >::Register( vm_ );
	Luna< CLuaGameBox >::Register( vm_ );
	Luna< CLuaGame >::Register( vm_ );
	Luna< CLuaIniFile >::Register( vm_ );
	Luna< CLuaDiskMachine >::Register( vm_ );
	Luna< CLuaPlantWork >::Register( vm_ );
	Luna< CLuaPlant >::Register( vm_ );
	Luna< CLuaReklame >::Register( vm_ );
	Luna< CLuaPictureFlow >::Register( vm_ );
	Luna< CLuaDevelopModule >::Register( vm_ );
	Luna< CLuaDevelopProject >::Register( vm_ );
	Luna< CLuaTechMap >::Register( vm_ );
	Luna< CLuaBrowser >::Register( vm_ );
	Luna< CLuaInvention >::Register( vm_ );
	Luna< CLuaAutoScript >::Register( vm_ );
	Luna< CLuaTab >::Register( vm_ );
	Luna< CLuaRelation >::Register( vm_ );
	Luna< CLuaPda >::Register( vm_ );
	Luna< CLuaPlatform >::Register( vm_ );
	Luna< CLuaFlick >::Register( vm_ );
	Luna< CLuaAnimator >::Register( vm_ );
	Luna< CLuaSoundEngine >::Register( vm_ );
	Luna< CLuaGuiLink >::Register( vm_ );
	Luna< CLuaQuest >::Register( vm_ );
	Luna< CLuaQuestEngine >::Register( vm_ );
}

CNrpScript::~CNrpScript()
{
	// ���������� �� ������ ����������� ����������
	try
	{
		lua_close(vm_);
		// �������������� ���������
		vm_ = 0;
	}
	catch (...)
	{
		// ������ �� ������, ��� ��� ������
	}


}

void CNrpScript::LoadFile( const NrpText& fileName )
{
	Param( LOAD_FUNCTIONS_FILENAME ) = fileName;

	if ( luaL_loadfile( vm_, const_cast< NrpText& >( fileName ) ) != 0)
	{
		// ����������� ��������� �� ������
		NrpText errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		if (errMsg.size())
			Log(SCRIPT, FATAL)  << errMsg  << term;
		else
			Log(SCRIPT, FATAL)  << "����������� ������ ������� \"" << fileName << "\"" << term;
	}
	else
		lua_pcall( vm_, 0, LUA_MULTRET, 0 );
}

bool CNrpScript::DoFile( const NrpText& fileName )
{
	// !0 � ������ ������ � �������
	if (luaL_dofile(vm_, const_cast< NrpText& >( fileName ) ) != 0)
	{
		// ����������� ��������� �� ������
		NrpText errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		if( errMsg.size() )
			Log(SCRIPT, FATAL)  << errMsg << term;
		else
			Log(SCRIPT, FATAL)  << "����������� ������ ������� \"" << fileName  << term;

		return 1;
	}

	return 0;
}

void CNrpScript::DoString( const NrpText& s )
{
	try
	{
		if (luaL_dostring(vm_, const_cast< NrpText& >( s ).ToStr() ) != 0)
		{
			// ����������� ��������� �� ������
			NrpText errMsg = lua_tostring(vm_, -1);
			// ������ �� ����� ��������� �� ������
			lua_pop(vm_, -1);
			Log(SCRIPT, FATAL) << errMsg << term;
		}
	}
	catch(...)
	{
		NrpText errMsg = lua_tostring(vm_, -1);
		// ������ �� ����� ��������� �� ������
		lua_pop(vm_, -1);
		Log(SCRIPT, FATAL) << errMsg << term;
	}
}

CNrpScript& CNrpScript::Instance()
{
	if( global_script_engine == NULL )
		global_script_engine = new CNrpScript();
	
	return *global_script_engine; 
}

//! �������� ��������� ������� �� ����������� ����������
void CNrpScript::CallFunction( int funcRef, void* sender, void* userData )
{
	try
	{
		//DumpStack( _vm);

		// ������ �� ������� ����� ������� � ��������� ������
		lua_getref( vm_, funcRef );
		lua_pushlightuserdata( vm_, sender );
		lua_pushlightuserdata( vm_, userData );
		//DumpStack( _vm );

		if( lua_pcall( vm_, 2, LUA_MULTRET, 0 ) != 0 )
		{
			// ����������� ��������� �� ������
			NrpText errMsg = lua_tostring(vm_, -1);

			lua_pop(vm_, -1);
			Log(SCRIPT, FATAL) << errMsg << term;
		}
	}
	catch(...)
	{}
}

void CNrpScript::CallFunction( const NrpText& funcName, void* sender, void* userData )
{
	lua_getfield( vm_, LUA_GLOBALSINDEX, const_cast< NrpText& >( funcName ) );
	lua_pushlightuserdata( vm_, sender );
	lua_pushlightuserdata( vm_, userData );

	if( lua_pcall( vm_, 2, LUA_MULTRET, 0 ) != 0 )
	{
		// ����������� ��������� �� ������
		NrpText errMsg = lua_tostring(vm_, -1);

		lua_pop(vm_, -1);
		Log(SCRIPT, FATAL) << errMsg << term;
	}
}

void CNrpScript::TemporaryScript( const NrpText& fileName, SCRIPT_ACTION action )
{
	switch( action )
	{
	case SA_CREATE:
		{
			NrpText fn = NrpText("tmp/") + fileName + ".lua";
			IWriteFile* file = _nrpEngine.GetFileSystem()->createAndWriteFile( fn );
			file->drop();	
		}
	break;

	case SA_EXEC:
		{
			NrpText fn = NrpText("tmp/") + fileName + ".lua";
			DoFile( fn );
		}
	}
}

void CNrpScript::AddActionToTemporaryScript( const NrpText& fileName, const NrpText& action )
{
	NrpText fn = NrpText("tmp/") + fileName + ".lua";
	IWriteFile* file = _nrpEngine.GetFileSystem()->createAndWriteFile( fn, true );
	file->write( const_cast< NrpText& >( action ).ToStr(), action.size() );
	file->write( "\n", 1 );
	file->drop();		
}

NrpText CNrpScript::ClassName()
{
	return CLASS_NRPSCRIPT;
}

void CNrpScript::ReleaseRef( int action )
{
	lua_unref( vm_, action );
}

}//namespace nrp