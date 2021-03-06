#pragma once
#include "nrpconfig.h"

namespace nrp
{

OPTION_NAME BASEDIR("basedir");
class CNrpHtmlEngineConfig : public INrpConfig
{
private:
	CNrpHtmlEngineConfig();

public:
    static const NrpText defaultConfig;

	static CNrpHtmlEngineConfig& Instance();
	static NrpText ClassName();
};

}//namespace nrp