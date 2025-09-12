///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CXMFEditor.h"

///////////////////////////////////////////////////////////////////////////

CXMFWindow::CXMFWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 640,480 ), wxDefaultSize );

	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFileImport;
	m_menuFileImport = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Import") ) + wxT('\t') + wxT("Ctrl+I"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileImport );

	wxMenuItem* m_menuFileOpen;
	m_menuFileOpen = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileOpen );

	wxMenuItem* m_menuFileSave;
	m_menuFileSave = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileSave );
	m_menuFileSave->Enable( false );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuFileExit;
	m_menuFileExit = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Exit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileExit );

	m_menubar->Append( m_menuFile, _("File") );

	m_menuInfo = new wxMenu();
	wxMenuItem* m_menuInfoVersion;
	m_menuInfoVersion = new wxMenuItem( m_menuInfo, wxID_ANY, wxString( _("Version") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuInfo->Append( m_menuInfoVersion );

	m_menubar->Append( m_menuInfo, _("Info") );

	this->SetMenuBar( m_menubar );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_ModelMainPanel = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_ModelPanel_Settings = new wxPanel( m_ModelMainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );


	bSizer7->Add( 0, 10, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( m_ModelPanel_Settings, wxID_ANY, _("Model name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer8->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrlModelName = new wxTextCtrl( m_ModelPanel_Settings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrlModelName->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlModelName->SetMaxLength( 32 );
	}
	#else
	m_textCtrlModelName->SetMaxLength( 32 );
	#endif
	m_textCtrlModelName->SetToolTip( _("Specify the model name.\n\nIf this field is empty, the model will be named as the file.") );

	bSizer8->Add( m_textCtrlModelName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer8->Add( 5, 0, 0, wxEXPAND, 5 );

	m_staticText31 = new wxStaticText( m_ModelPanel_Settings, wxID_ANY, _("Skinned:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer8->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );

	m_TextSkinned = new wxStaticText( m_ModelPanel_Settings, wxID_ANY, _("OFF"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TextSkinned->Wrap( -1 );
	bSizer8->Add( m_TextSkinned, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );


	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );


	bSizer7->Add( 0, 5, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_staticText6 = new wxStaticText( m_ModelPanel_Settings, wxID_ANY, _("Meshes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer9->Add( m_staticText6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticline2 = new wxStaticLine( m_ModelPanel_Settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );


	bSizer81->Add( bSizer9, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_staticText5 = new wxStaticText( m_ModelPanel_Settings, wxID_ANY, _("Bones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer10->Add( m_staticText5, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticline21 = new wxStaticLine( m_ModelPanel_Settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer10->Add( m_staticline21, 0, wxEXPAND | wxALL, 5 );


	bSizer81->Add( bSizer10, 1, wxEXPAND, 5 );


	bSizer7->Add( bSizer81, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );

	m_listMesh = new wxListbook( m_ModelPanel_Settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listMeshListView = m_listMesh->GetListView();
	long m_listMeshFlags = m_listMeshListView->GetWindowStyleFlag();
	if( m_listMeshFlags & wxLC_SMALL_ICON )
	{
		m_listMeshFlags = ( m_listMeshFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	m_listMeshListView->SetWindowStyleFlag( m_listMeshFlags );
	#endif

	bSizer71->Add( m_listMesh, 1, wxEXPAND | wxALL, 5 );

	m_staticline1 = new wxStaticLine( m_ModelPanel_Settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer71->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );

	m_listBones = new wxListbook( m_ModelPanel_Settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listBonesListView = m_listBones->GetListView();
	long m_listBonesFlags = m_listBonesListView->GetWindowStyleFlag();
	if( m_listBonesFlags & wxLC_SMALL_ICON )
	{
		m_listBonesFlags = ( m_listBonesFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	m_listBonesListView->SetWindowStyleFlag( m_listBonesFlags );
	#endif

	bSizer71->Add( m_listBones, 1, wxEXPAND | wxALL, 5 );


	bSizer7->Add( bSizer71, 1, wxEXPAND, 5 );


	m_ModelPanel_Settings->SetSizer( bSizer7 );
	m_ModelPanel_Settings->Layout();
	bSizer7->Fit( m_ModelPanel_Settings );
	m_ModelMainPanel->AddPage( m_ModelPanel_Settings, _("Settings"), false );

	bSizer4->Add( m_ModelMainPanel, 1, wxEXPAND | wxALL, 0 );


	this->SetSizer( bSizer4 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CXMFWindow::onMenuSelect_File_Import ), this, m_menuFileImport->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CXMFWindow::onMenuSelect_File_Open ), this, m_menuFileOpen->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CXMFWindow::onMenuSelect_File_Save ), this, m_menuFileSave->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CXMFWindow::onMenuSelect_File_Exit ), this, m_menuFileExit->GetId());
	m_menuInfo->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CXMFWindow::onMenuSelect_Info_Version ), this, m_menuInfoVersion->GetId());
}

CXMFWindow::~CXMFWindow()
{
}

CXMFImportDlg::CXMFImportDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 170,200 ), wxSize( 170,200 ) );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );

	m_checkBoxGSN = new wxCheckBox( this, wxID_ANY, _("GenSmoothNormals"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxGSN->SetValue(true);
	m_checkBoxGSN->SetToolTip( _("Generates smooth normals for all vertices in the mesh.\n\nThis is ignored if normals are already there at the time this flag is evaluated. Model importers try to load them from the source file, so they're usually already there.\n\nUsing a decent angle here (e.g. 80 degrees) results in very good visual appearance.") );

	bSizer21->Add( m_checkBoxGSN, 0, wxALL, 10 );

	m_staticTextSliderGSN = new wxStaticText( this, wxID_ANY, _("Smooth angle"), wxPoint( -1,-1 ), wxDefaultSize, 0 );
	m_staticTextSliderGSN->Wrap( -1 );
	m_staticTextSliderGSN->SetToolTip( _("Specifies the maximum angle that may be between two face normals at the same vertex position that their are smoothed together.\n\nThis applies to the GenSmoothNormals. The angle is specified in degrees, so 180 is PI. The default value is 175 degrees (all vertex normals are smoothed). The maximum value is 175, too.") );

	bSizer21->Add( m_staticTextSliderGSN, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0 );

	m_sliderGSN = new wxSlider( this, wxID_ANY, 175, 5, 175, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	bSizer21->Add( m_sliderGSN, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	mainSizer->Add( bSizer21, 1, wxEXPAND, 0 );


	mainSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );

	m_buttonOk = new wxButton( this, wxID_ANY, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( m_buttonOk, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	mainSizer->Add( bSizer24, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CXMFImportDlg::onSettings_Ok ), NULL, this );
}

CXMFImportDlg::~CXMFImportDlg()
{
}
