/********************************************************************
	created:	2010/01/28
	created:	28:1:2010   9:57
	filename: 	nrpConfig.h
	file base:	nrpConfig
	file ext:	h
	author:		Dalerank
	
	purpose:	�������� ������� �������
*********************************************************************/
#pragma once

#include <map>
#include <string>
#include <typeinfo.h>

#include "IniFile.h"
#include "INrpObject.h"

namespace nrp
{

typedef const std::string OPTION_NAME;
typedef const std::string ELEMENT_ALIGN;
OPTION_NAME CONFIG_FILE( "configFile" );

ELEMENT_ALIGN ALIGN_TOP( "top" );
ELEMENT_ALIGN ALIGN_BOTTOM( "bottom" );
ELEMENT_ALIGN ALIGN_MIDDLE( "middle" );
ELEMENT_ALIGN ALIGN_LEFT( "left" );
ELEMENT_ALIGN ALIGN_RIGTH( "rigth" );
ELEMENT_ALIGN ALIGN_CENTER( "center" );

OPTION_NAME BALANCE("balance");
OPTION_NAME NAME("name");
OPTION_NAME TYPE( "type" );
OPTION_NAME QUALITY( "quality" );
OPTION_NAME READYWORKPERCENT( "readyWorkPercent" );
OPTION_NAME TECHNUMBER( "techNumber" );
OPTION_NAME USERNUMBER( "userNumber" );
OPTION_NAME COMPANIESNUMBER( "companiesNumber" );
OPTION_NAME COMPANY( "company" );
OPTION_NAME PROPERTIES( "properties" );

#define CHECK_VALCLASS_TYPE( bclass )\
	if( valueType_ != typeid( bclass ).name() ) {\
		std::string warn( "warning: request type " );\
		warn +=	std::string( typeid( bclass ).name()) + " but native typename is " +valueType_ + "\n";\
		OutputDebugString( warn.c_str() );\
	}

class INrpProperty
{
public:
	virtual std::string GetValueType() = 0;
	virtual ~INrpProperty()
	{

	}
};
typedef std::map< OPTION_NAME, INrpProperty* > PropertyArray;

class INrpConfig;

template< class ValClass > class CNrpProperty : public INrpProperty
{
	friend INrpConfig;
public:
	CNrpProperty( ValClass pValue )
	{
		valueType_ = typeid( ValClass ).name();
		ptrValue_ = new ValClass( pValue );
	}

	virtual ~CNrpProperty() 
	{ 
		delete ptrValue_; 
	}

	std::string GetValueType() { return valueType_; }

	ValClass& ToggleValue()
	{
		*ptrValue_ = !(*ptrValue_);
		return *ptrValue_;
	}

	ValClass& GetValue() { return *ptrValue_; }

	template<class B> B& GetValue()
	{
#ifdef _DEBUG		
		CHECK_VALCLASS_TYPE( B )
#endif
		return *(B*)ptrValue_;
	}

	template<class B> void SetValue( B valuel )
	{
#ifdef _DEBUG		
		CHECK_VALCLASS_TYPE( B )
#endif
		*(B*)ptrValue_ = valuel ;
	}

private:
	std::string valueType_;
	ValClass* ptrValue_;

	CNrpProperty() {};
	CNrpProperty( CNrpProperty& ) {};
};

class INrpConfig : public INrpObject
{
protected:
	//! ������ �������� �� ����������������� �����
	template< class B > B Read_( std::string section, std::string key, B def_value )
	{
		B read_value;
		
		read_value = nrp::IniFile::Read( section, key, def_value, GetValue< std::string >( CONFIG_FILE ) );

		return (B)read_value;
	}

	//! �������� ������� �������
	virtual void Load_( char* file_name ) = 0;

public:
	INrpConfig( CLASS_NAME className, SYSTEM_NAME sysName ) : INrpObject( className, sysName )
	{

	}

	//! ��� ������ �������
	CLASS_NAME GetType() { return ClassName(); }

	PropertyArray& GetProperties() { return options_; }

	void EraseValue( std::string name ) const
	{
		PropertyArray::const_iterator pIter = options_.find( name );

		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string errstr = "erase: bad config param " + name + " \n"; 
			OutputDebugString( errstr.c_str() );
#endif
			throw "error"; 
		}
		else 
			delete pIter->second;
	}

	template< class B > void CreateValue( std::string name, B valuel )
	{
		options_[ name ] = new CNrpProperty<B>( valuel );
	}

	template< class B > B ToggleValue( std::string name, B defValue )
	{
		PropertyArray::iterator pIter = options_.find( name );
		if( pIter == options_.end() )
		{
			CreateValue<B>( name, defValue );
			return defValue;
		}
		else
			((CNrpProperty<B>*)pIter->second)->ToggleValue();

		return ((CNrpProperty<B>*)pIter->second)->GetValue<B>();
	}

	//! ��������� �������� �������
	template< class B > B& GetValue( std::string name ) const
	{
		PropertyArray::const_iterator pIter = options_.find( name );
		
		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string errstr = "read: bad config param " + name + " \n"; 
			OutputDebugString( errstr.c_str() );
#endif
			throw "error"; 
		}
		else 
			return ((CNrpProperty<B>*)pIter->second)->GetValue<B>();
	}

	template< class B > void SetValue( std::string name, B valuel ) 
	{ 
		PropertyArray::iterator pIter = options_.find( name );

		if( pIter == options_.end() )
		{
#ifdef _DEBUG
			std::string text = "write: bad config param " + ClassName() + ":" + name;
			OutputDebugString( text.c_str() );

			throw text.c_str();
#endif
		}
		else 
			((CNrpProperty<B>*)pIter->second)->SetValue( valuel );
	}

	virtual void Save( std::string scetionName, std::string fileName )
	{
		PropertyArray::iterator paIter = options_.begin();
		for( ; paIter != options_.end(); ++paIter)
		{
			INrpProperty* prop = paIter->second;
			if( prop->GetValueType() == typeid( int ).name() )
				IniFile::Write( scetionName, paIter->first, ((CNrpProperty<int>*)prop)->GetValue(), fileName );
			else
			if( prop->GetValueType() == typeid( std::string ).name() )
				IniFile::Write( scetionName, paIter->first, ((CNrpProperty<std::string>*)prop)->GetValue(), fileName );
			else
			if( prop->GetValueType() == typeid( bool ).name() )
				IniFile::Write( scetionName, paIter->first, std::string(((CNrpProperty<bool>*)prop)->GetValue() ? "true" : "false"), fileName );
			else
			if( prop->GetValueType() == typeid( SYSTEMTIME ).name() )
				IniFile::Write( scetionName, paIter->first, ((CNrpProperty<SYSTEMTIME>*)prop)->GetValue(), fileName );
			else 
			if( prop->GetValueType() == typeid( float ).name() )
				IniFile::Write( scetionName, paIter->first, ((CNrpProperty<float>*)prop)->GetValue(), fileName );
		}
	}

	virtual void Load( std::string sectionName, std::string fileName )
	{
		char buffer[ 32000 ];
		memset( buffer, 0, 32000 );
		GetPrivateProfileSection( sectionName.c_str(), buffer, 32000, fileName.c_str() );

		std::string readLine = buffer;
		while( readLine != "" )
		{
			std::string name, valuel;
			name = readLine.substr( 0, readLine.find( '=' ) );
			valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );
			if( IsNumber( valuel.c_str() ) )
				CreateValue<int>( name, StrToInt( valuel.c_str() ) );
			else
			if( IsFloatNumber( valuel.c_str() ) )
				CreateValue<float>( name, StrToFloat( valuel.c_str() ) ); 
			else 
			{
				if( valuel == "true" || valuel == "false" )
					CreateValue<bool>( name, valuel == "true" );
				else
					CreateValue<std::string>( name, valuel );
			}
			memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
			readLine = buffer;
		}
	}

private:
	//! ����������� ������� �������
	PropertyArray options_;
};

} //namespace nrp