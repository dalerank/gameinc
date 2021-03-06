#pragma once
#include <irrlicht.h>
#include <llmozlib2.h>
#include "NrpText.h"

namespace irr
{
	namespace gui
	{
		class CNrpBrowserWindow;
	}

	namespace video
	{
		class ITexture;
	}
}

namespace nrp
{

/*! ����� ���-������ Gecko 1.8.1
	��������� �������������� � ������� � ����������� ������
*/
class CNrpGame;

class HTMLEngine : public LLEmbeddedBrowserWindowObserver
{
public:
    static const NrpText nofollowScheme;
    static const NrpText fileScheme;
    static const NrpText screenshotSrcTemplate;
    static const int maxScrNumber;
	//*! ������������� ������ ��� ������ ������
	static HTMLEngine& Instance();
	
	irr::gui::CNrpBrowserWindow& GetBrowserWindow(irr::core::dimension2du size=irr::core::dimension2du( 512, 512 ) );
	void SetBrowserWindow( irr::gui::CNrpBrowserWindow* newWindow ) { browserWindow_ = newWindow; }
	void SetPage404( const NrpText& pageUrl );
	void Update();

	//*! �������, ������� ��������� ��� ���������� ��������� �� ������
	void onPageChanged( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! ������� ���������� ����� ������ ���������
	void onNavigateBegin( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! ������� ���������� ����� ��������� ���������
	void onNavigateComplete( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onUpdateProgress( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onStatusTextChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	void onLocationChange( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! ������� ����������, ����� ������������ ������� �� ����� ������
	void onClickLinkHref( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! ������� ����������, ����� ������������ ������� �� ������������� ������... �������� http://dosomething
	void onClickLinkNoFollow( const LLEmbeddedBrowserWindowEvent& eventIn );
	//*! �������� ������ ������� �� �������� ������
	void Navigate( const NrpText& url );
	//*! �������� � ���-������ ������� ���
	void MouseDown( size_t x, size_t y );
	//*! �������� � ��� ������ ������� ���
	void MouseUp(size_t x, size_t y);
	void ScrollByLines( int lines );
	void SetFocus( bool focus );
	void KeyPress( int key );
	void MouseMoved( size_t x, size_t y );

    void CreateDescription( const NrpText& templateFile, const NrpText& fileName, const CNrpGame& game );

	static NrpText ClassName() { return "CHtmlEngine"; }
private:

	HTMLEngine();
	HTMLEngine(const HTMLEngine &other);
	~HTMLEngine();
	//*! ��������, ���� ����������� ����������� ������������ ��������
	irr::video::ITexture* pageTexture_;
	int browserWindowId_;
	size_t browserRasterSize_;
	size_t _lastTimeUpdate;

	NrpText _page404;
	irr::gui::CNrpBrowserWindow* browserWindow_;

	bool dataUpdated_;
	//*! ���������� �������
	LLMozLib* llmozlib_;
	bool _noFollowLinkExec, _firstMessage;

	CRITICAL_SECTION rasterCs_;
};

}