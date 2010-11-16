#include "StdAfx.h"
#include "NrpWorldConfig.h"

using namespace irr;
static nrp::CNrpWorldConfig * globalTerrainConfigInstance = 0;

namespace nrp 
{
CLASS_NAME CLASS_WORLDCONFIG( "CNrpWorldConfig" );

CNrpWorldConfig::CNrpWorldConfig(void) : INrpConfig( CLASS_WORLDCONFIG, CLASS_WORLDCONFIG )
{
	CreateValue<core::dimension2df>(WORLD_REAL_SIZE, core::dimension2df( 0, 0 ) );
	CreateValue<core::vector3df>( WORLD_MINEDGE, core::vector3df( 99999, 99999, 99999 ) );
	CreateValue<core::vector3df>( WORLD_MAXEDGE, core::vector3df( -99999, -99999, -99999 ) );
	CreateValue<float>( WORLD_WIDTH_COEFF, 1.f );

	Load( "config/world.ini" );
}

CNrpWorldConfig& CNrpWorldConfig::Instance()
{
	if( !globalTerrainConfigInstance)
		globalTerrainConfigInstance = new CNrpWorldConfig();

	return *globalTerrainConfigInstance;
}

NrpText CNrpWorldConfig::ClassName()
{
	return CLASS_WORLDCONFIG;
}
}//namespace nrp