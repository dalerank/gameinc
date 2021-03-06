#include "stdafx.h"
#include "ConfigMap.h"
#include <assert.h>

core::stringc CConfigMap::getConfig( const core::stringc& name )				//�������� ���� ������ �������� ��� ������ ������
{																				//���� �������� �� ���� �������
    core::map<core::stringc,core::stringc>::Node* node = _configs.find(name);		//

    if ( node == NULL )
        return core::stringc();

    return node->getValue();
}
//////////////////////////////////////////////////////////////////////

s32 CConfigMap::getConfigAsInt( const core::stringc& name )						//�������� ���������� int
{
    core::stringc cfg = getConfig(name);

    if ( cfg.size() == 0 )
        return 0;

    s32 x = 0;
    sscanf_s( cfg.c_str(), "%d", &x );

    return x;
}
//////////////////////////////////////////////////////////////////////

f32 CConfigMap::getConfigAsFloat( const core::stringc& name )
{
    core::stringc cfg = getConfig(name);

    if ( cfg.size() == 0 )
        return 0;

    f32 x = 0.0f;
    sscanf_s( cfg.c_str(), "%f", &x );

    return x;
}
//////////////////////////////////////////////////////////////////////

core::vector2df CConfigMap::getConfigAsVector2df( const core::stringc& name )
{
    core::stringc cfg = getConfig(name);

    if ( cfg.size() == 0 )
        return core::vector2df(0,0);

    core::vector2df vec;

    sscanf_s( cfg.c_str(), "%f , %f", &vec.X, &vec.Y );

    return vec;
}
//////////////////////////////////////////////////////////////////////

core::vector3df CConfigMap::getConfigAsVector3df( const core::stringc& name )
{
    core::stringc cfg = getConfig(name);

    if ( cfg.size() == 0 )
        return core::vector3df(0,0,0);

    core::vector3df vec;

    sscanf_s( cfg.c_str(), "%f , %f , %f", &vec.X, &vec.Y, &vec.Z );

    return vec;
}
//////////////////////////////////////////////////////////////////////

bool CConfigMap::hasConfig( const core::stringc& name )
{
    core::map<core::stringc,core::stringc>::Node* node = _configs.find(name);

    return ( node != 0 );
}

void CConfigMap::setConfig( const core::stringc& name, const core::stringc& value )
{
    int posDelim = name.findFirst( '.' );
    assert( posDelim > 0 );
    core::stringc controlName = name.subString( 0, posDelim );
 
    if( _controls.linear_search( controlName ) == -1 )
        _controls.push_back( controlName );

    _configs.set(name,value);
}
