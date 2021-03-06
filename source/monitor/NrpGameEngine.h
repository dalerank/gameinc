#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME AVGENRE_COUNT( "avGenreCount" );
OPTION_NAME TIME_ACTUAL( "timeActual" ); //������������ ������... ��������������� ��� ������ ����� ����������

class CNrpGameEngine : public INrpProject
{
public:
	CNrpGameEngine( const NrpText& name );
	CNrpGameEngine( const NrpText& fileName, bool load );

	void AddGenre( const NrpText& typen );
	int GetGenreCount() { return _techs.size(); }
	NrpText GetGenre( int index );
	bool IsMyTech( const NrpText& typen );
	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	~CNrpGameEngine(void);

	static NrpText ClassName();

private:
	KNOWLEDGE_MAP _techs;

	void _InitialyzeOptions();
};

typedef CNrpGameEngine* PNrpGameEngine;

}//namespace nrp