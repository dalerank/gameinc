#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME AVGENRE_COUNT( "avGenreCount" );
OPTION_NAME TIME_ACTUAL( "timeActual" ); //������������ ������... ��������������� ��� ������ ����� ����������
OPTION_NAME SKILL_CODING( "skillCoding" );

class CNrpGameEngine : public INrpProject
{
public:
	CNrpGameEngine( const NrpText& name );
	CNrpGameEngine( const NrpText& fileName, bool load );

	void AddGenre( GENRE_TYPE typen );
	int GetGenreCount() { return _avgenres.size(); }
	GENRE_TYPE GetGenre( int index );
	bool IsGenreAvailble( GENRE_TYPE typen );
	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	~CNrpGameEngine(void);

	static NrpText ClassName();

private:
	REQUIRE_MAP _avgenres;

	void _InitialyzeOptions();
};

typedef CNrpGameEngine* PNrpGameEngine;

}//namespace nrp