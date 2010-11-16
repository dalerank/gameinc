#pragma once
#include "nrpconfig.h"

namespace nrp
{
class CNrpHtmlEngineConfig : public INrpConfig
{
private:
	CNrpHtmlEngineConfig();

public:
	static CNrpHtmlEngineConfig& Instance();
	static NrpText ClassName();
};

}//namespace nrp