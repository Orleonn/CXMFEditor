///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFWindow
///////////////////////////////////////////////////////////////////////////////
class CXMFWindow : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxMenu* m_menuInfo;
		wxNotebook* m_ModelMainPanel;
		wxPanel* m_ModelPanel_Main;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlModelName;
		wxStaticText* m_staticText31;
		wxStaticText* m_TextSkinned;
		wxStaticText* m_staticText7;
		wxStaticText* m_TextTotalVerts;
		wxStaticText* m_staticText71;
		wxStaticText* m_TextTotalFaces;
		wxStaticText* m_staticText6;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText5;
		wxStaticLine* m_staticline21;
		wxListbook* m_listMesh;
		wxStaticLine* m_staticline1;
		wxListbook* m_listBones;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, override them in your derived class
		virtual void onWindowButton_Close( wxCloseEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Import( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Open( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Exit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_Info_About( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_Info_Github( wxCommandEvent& event ) { event.Skip(); }


	public:

		CXMFWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CXMF Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL );

		~CXMFWindow();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFImportDlg
///////////////////////////////////////////////////////////////////////////////
class CXMFImportDlg : public wxDialog
{
	private:

	protected:
		wxCheckBox* m_checkBoxGSN;
		wxStaticText* m_staticTextSliderGSN;
		wxSlider* m_sliderGSN;
		wxCheckBox* m_checkFStatic;
		wxButton* m_buttonOk;

		// Virtual event handlers, override them in your derived class
		virtual void onButton_Ok( wxCommandEvent& event ) { event.Skip(); }


	public:

		CXMFImportDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 170,220 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~CXMFImportDlg();

};

