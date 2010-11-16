#include "StdAfx.h"
#include "irrlicht.h"
#include "NrpConsoleConfig.h"

using namespace irr;

static nrp::CNrpConsoleConfig * globalConsoleConfigInstance = 0;

namespace nrp
{
CLASS_NAME CLASS_CONSOLECONFIG( "CNrpConsoleConfig" );

void CNrpConsoleConfig::_InitializeOptions()
{
	CreateValue<int>( CONSOLE_INIT_KEY, '`' );
	CreateValue<core::dimension2df>( CONSOLE_RELATIVE_SIZE, core::dimension2df( 0.9f, 0.6f ) );
	CreateValue<int>( CONSOLE_LINE_SPACING, 2 );
	CreateValue<int>( CONSOLE_INDENT, 1 );
	CreateValue<NrpText>( CONSOLE_VERT_ALIGN, "top" );
	CreateValue<NrpText>( CONSOLE_HORT_ALIGN, "center" );
	CreateValue<bool>( CONSOLE_SHOW_BACKGROUND, true );
	CreateValue<video::SColor>( CONSOLE_BG_COLOR, video::SColor( 150,10,10,70) );
	CreateValue<video::SColor>( CONSOLE_FONT_COLOR, video::SColor( 200,200,200,200 ) );
	CreateValue<NrpText>( CONSOLE_FONTNAME, "font_8" ); 
	CreateValue<NrpText>( CONSOLE_PROMT, "nrp" );
	CreateValue<int>( CONSOLE_HISTORY_SIZE, 10 );
	CreateValue<int>( CONSOLE_MAX_BLEND, 128 );
	CreateValue<NrpText>( CONSOLE_COMMANDS_FILE, "console.commands" );
	CreateValue<NrpText>( CONSOLE_GUINAME, "SystemConsole" );
}

CNrpConsoleConfig::CNrpConsoleConfig(void) : INrpConfig( CLASS_CONSOLECONFIG, CLASS_CONSOLECONFIG )
{
	_InitializeOptions();

	Load( "config/console.ini" );	
}

CNrpConsoleConfig& CNrpConsoleConfig::Instance()
{
	if( !globalConsoleConfigInstance)
		globalConsoleConfigInstance = new CNrpConsoleConfig();

	return *globalConsoleConfigInstance;
}

NrpText CNrpConsoleConfig::ClassName()
{
	return CLASS_CONSOLECONFIG;
}
}//namespace nrp
