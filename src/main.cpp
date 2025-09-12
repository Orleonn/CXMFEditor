
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif



class MainApp : public wxApp  // MainApp is the class for our application
{
	// MainApp just acts as a container for the window, or frame in MainFrame

public:
	virtual bool OnInit();
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

#include "CXMFEditor.h"

bool MainApp::OnInit()
{
	// Create an instance of our frame, or window
	MainFrame* MainWin = new MainFrame(_("Hello World!"), wxDefaultPosition, wxSize(300, 200));
	MainWin->Show(true);	// show the window
	SetTopWindow(MainWin);	// and finally, set it as the main window
	return true;
}

