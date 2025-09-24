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
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFWindowBase
///////////////////////////////////////////////////////////////////////////////
class CXMFWindowBase : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar;
		wxMenu* m_menuFile;
		wxMenu* m_menuImport;
		wxMenu* m_menuOpen;
		wxMenu* m_menuInfo;
		wxNotebook* m_MainNotebook;
		wxPanel* m_MainPanel_Model;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlModelName;
		wxStaticText* m_staticText31;
		wxStaticText* m_TextSkinned;
		wxStaticText* m_staticText7;
		wxStaticText* m_TextTotalVerts;
		wxStaticText* m_staticText71;
		wxStaticText* m_TextTotalFaces;
		wxButton* m_button4;
		wxStaticLine* m_staticline2;
		wxListbook* m_listMesh;
		wxStaticLine* m_staticline1;
		wxListbook* m_listBones;
		wxPanel* m_MainPanel_SkAnim;
		wxButton* m_button7;
		wxListbook* m_listSkAnims;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, override them in your derived class
		virtual void onWindowButton_Close( wxCloseEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_ImportModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_ImportSkAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_OpenModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_OpenSkAnim( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Save( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_File_Exit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_Info_Preview( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_Info_About( wxCommandEvent& event ) { event.Skip(); }
		virtual void onMenuSelect_Info_Github( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnModelNameChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButton_DumpModel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButton_DumpSkAnim( wxCommandEvent& event ) { event.Skip(); }


	public:

		CXMFWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CXMF Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL );

		~CXMFWindowBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFImportDlgBase
///////////////////////////////////////////////////////////////////////////////
class CXMFImportDlgBase : public wxDialog
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

		CXMFImportDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import Settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 170,220 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~CXMFImportDlgBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFTextDialogBase
///////////////////////////////////////////////////////////////////////////////
class CXMFTextDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_TextCap;
		wxTextCtrl* m_textCtrl;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnButton_Ok( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButton_Cancel( wxCommandEvent& event ) { event.Skip(); }


	public:

		CXMFTextDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 240,140 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~CXMFTextDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CXMFDumpDlgBase
///////////////////////////////////////////////////////////////////////////////
class CXMFDumpDlgBase : public wxDialog
{
	private:

	protected:
		wxTextCtrl* m_textCtrl;
		wxButton* m_button;
		wxButton* m_button2;

		// Virtual event handlers, override them in your derived class
		virtual void OnButton_Ok( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButton_Copy( wxCommandEvent& event ) { event.Skip(); }


	public:

		CXMFDumpDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 680,640 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );

		~CXMFDumpDlgBase();

};

