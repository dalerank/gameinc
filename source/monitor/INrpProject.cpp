#include "StdAfx.h"
#include "INrpProject.h"

namespace nrp
{
	
INrpProject::INrpProject( const NrpText& className, const NrpText& systemName ) : INrpConfig( className, systemName )
{
	RegProperty( NAME, NrpText("") );
	RegProperty( INTERNAL_NAME, NrpText("") );
	RegProperty( TECHGROUP, (int)PT_UNKNOWN );
}

INrpProject::~INrpProject(void)
{
}

NrpText INrpProject::Save( const NrpText& fileName )
{
	return INrpConfig::Save( fileName );
}

bool INrpProject::Equale( const NrpText& name )
{
	return ( Param( NAME ) == name || Param( INTERNAL_NAME ) == name);
}

}//namespace nrp