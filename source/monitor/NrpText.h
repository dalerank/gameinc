/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   10:03
	file base:	NrpText
	file ext:	h
	author:		Dalerank
	
	purpose:	������� ��� ������ �� �������� ANSI � UNICODE
*********************************************************************/
#pragma once
#include <sstream>
#include <irrlicht.h>

using namespace irr;
using namespace core;

namespace nrp															
{

class NrpText : public stringw
{
public:
	NrpText();
	NrpText( const stringc& str );
	NrpText( const stringw& str );
	NrpText( const char* str );
	NrpText( const wchar_t* str );
	NrpText( const SYSTEMTIME& time );
	NrpText( bool amount );
	NrpText( int amount );
	NrpText( float amount );
	NrpText( const vector3df& vec );
	NrpText( const recti& rectangle );
	NrpText( const dimension2di& dim );
	NrpText( const dimension2df& dim );
	NrpText( const dimension2du& dim );

	operator const char* ();
	operator const wchar_t* ();
	operator std::string ();
	
	NrpText& operator = ( const char* );
	NrpText& operator = ( const wchar_t* );
	NrpText& operator = ( const core::stringw& );

	NrpText operator+( const wchar_t* ) const;
	NrpText operator+( const NrpText& ) const;
	NrpText operator+( int amount ) const;

	void Conv2Hex();
	NrpText& FromHex( const char* ptr_hex );

	//������������� ������ � ���� ����������� �������
	vector3df ToVector3df();
	dimension2di ToDim2di();
	dimension2df ToDim2df();
	dimension2du ToDim2du();
	NrpText ToLower();
	NrpText ToLower() const;
	recti ToRect();
	
	//������� ������� ������ � �����
	bool IsNumber();
	bool IsFloat();
	float ToFloat();
	int ToInt();											//������� ������ � �����
	SYSTEMTIME ToTime();
	bool ToBool();

	const char* ToStr();
	const wchar_t* ToWide() { return c_str(); }
	const wchar_t* ToWide() const { return c_str(); }

private:
	void _FromStr( const char* str );
	stringc _append;
};//end namespace conv

typedef const NrpText OPTION_NAME;

}; //namespace nrp

using nrp::NrpText;