
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "CXMFEditor.h"
#include "CXMF.hpp"
#include "ModelPreview.hpp"

#include <filesystem>
#include <format>



class CXMFImportDialogWindow final : public CXMFImportDlg
{
private:
	using Super = CXMFImportDlg;

private:
	void onButton_Ok(wxCommandEvent& event) override;

public:
	CXMFImportDialogWindow() = delete;

	explicit CXMFImportDialogWindow(wxWindow* parent);
	~CXMFImportDialogWindow();

public:
	bool GSNEnabled() const
	{
		return Super::m_checkBoxGSN->GetValue();
	}

	float GetGSNAngle() const
	{
		return static_cast<float>(Super::m_sliderGSN->GetValue());
	}

	bool ForceStaticEnabled() const
	{
		return Super::m_checkFStatic->GetValue();
	}
};

CXMFImportDialogWindow::CXMFImportDialogWindow(wxWindow* parent)
	: Super(parent)
{
	//
}

CXMFImportDialogWindow::~CXMFImportDialogWindow()
{
	//
}

void CXMFImportDialogWindow::onButton_Ok(wxCommandEvent& /* event */)
{
	this->AcceptAndClose();
}



class MainWindow final : public CXMFWindow
{
private:
	using Super = CXMFWindow;

private:
	PreviewWindow* m_PreviewWnd;
	cxmf::IModel* m_Model;
	bool m_HasChanges;

private:
	void _destroy_model();
	void _show_controls(bool b);

	void _add_mesh_name_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName);
	void _add_texture_path_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName);
	void _add_vertex_counter(wxPanel* panel, wxBoxSizer* sizer, size_t vertexCount);
	void _add_face_counter(wxPanel* panel, wxBoxSizer* sizer, size_t faceCount);

	void _on_mesh_name_text_update(wxCommandEvent& event);
	void _on_texture_path_text_update(wxCommandEvent& event);

	void _fill_as_static();
	void _fill_as_skinned();

	void set_total_verticies(size_t count);
	void set_total_faces(size_t count);
	void set_status_text(const std::string& text);

	bool has_changes() const
	{
		return m_HasChanges;
	}

	void mark_changes()
	{
		m_HasChanges = true;
	}
	void unmark_changes()
	{
		m_HasChanges = false;
	}

private:
	void onWindowButton_Close(wxCloseEvent& event) override;
	void onMenuSelect_File_Import(wxCommandEvent& event) override;
	void onMenuSelect_File_Open(wxCommandEvent& event) override;
	void onMenuSelect_File_Save(wxCommandEvent& event) override;
	void onMenuSelect_File_Exit(wxCommandEvent& event) override;
	void onMenuSelect_Info_Preview(wxCommandEvent& event) override;
	void onMenuSelect_Info_About(wxCommandEvent& event) override;
	void onMenuSelect_Info_Github(wxCommandEvent& event) override;

private:
	void StartModelImport();
	void StartModelOpen();
	void StartModelSave();
	void StartClosing();

	void Reset();
	void SetModel(cxmf::IModel* model);

public:
	MainWindow();
	~MainWindow();
};

MainWindow::MainWindow()
	: Super(nullptr), m_PreviewWnd(nullptr), m_Model(nullptr), m_HasChanges(false)
{
	m_PreviewWnd = new PreviewWindow(Super::m_ModelMainPanel);
	Super::m_ModelMainPanel->AddPage(m_PreviewWnd, "Viewport", false);

	this->Show(true);
}

MainWindow::~MainWindow()
{
	_destroy_model();
}

void MainWindow::_destroy_model()
{
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}
}

void MainWindow::_show_controls(bool b)
{
	wxMenuItem* const item = Super::m_menuFile->GetMenuItems()[2];
	if (item) item->Enable(b);
	Super::m_ModelMainPanel->Enable(b);
}

void MainWindow::_add_mesh_name_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName)
{
	wxBoxSizer* const nameCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const nameCapText = new wxStaticText(panel, wxID_ANY, "Name");
	nameCapText->Wrap(-1);
	nameCapSizer->Add(nameCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	wxTextCtrl* const nameCapCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1));
	nameCapCtrl->SetValue(baseName);
	nameCapCtrl->SetToolTip("Mesh name.");
	nameCapSizer->Add(nameCapCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(nameCapSizer, 0, wxEXPAND, 5);
	nameCapCtrl->SetClientData(panel);
	nameCapCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MainWindow::_on_mesh_name_text_update),	//
						 nullptr, this);
}

void MainWindow::_add_texture_path_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName)
{
	wxBoxSizer* const nameCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const nameCapText = new wxStaticText(panel, wxID_ANY, "Texture path");
	nameCapText->Wrap(-1);
	nameCapSizer->Add(nameCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	wxTextCtrl* const nameCapCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1));
	nameCapCtrl->SetValue(baseName);
	nameCapCtrl->SetToolTip("Base texture path prefix.");
	nameCapSizer->Add(nameCapCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(nameCapSizer, 0, wxEXPAND, 5);
	nameCapCtrl->SetClientData(panel);
	nameCapCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MainWindow::_on_texture_path_text_update),  //
						 nullptr, this);
}

void MainWindow::_add_vertex_counter(wxPanel* panel, wxBoxSizer* sizer, size_t vertexCount)
{
	const std::string countStr = std::to_string(vertexCount);
	wxBoxSizer* const vertCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const vertCapText = new wxStaticText(panel, wxID_ANY, "Verticies:");
	vertCapText->Wrap(-1);
	vertCapSizer->Add(vertCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	wxStaticText* const vertCounter = new wxStaticText(panel, wxID_ANY, countStr);
	vertCapSizer->Add(vertCounter, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(vertCapSizer, 0, wxEXPAND, 5);
}

void MainWindow::_add_face_counter(wxPanel* panel, wxBoxSizer* sizer, size_t faceCount)
{
	const std::string countStr = std::to_string(faceCount);
	wxBoxSizer* const faceCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const faceCapText = new wxStaticText(panel, wxID_ANY, "Faces:");
	faceCapText->Wrap(-1);
	faceCapSizer->Add(faceCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	wxStaticText* const faceCounter = new wxStaticText(panel, wxID_ANY, countStr);
	faceCapSizer->Add(faceCounter, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(faceCapSizer, 0, wxEXPAND, 5);
}

void MainWindow::_on_mesh_name_text_update(wxCommandEvent& event)
{
	wxTextCtrl* const ctrl = wxDynamicCast(event.GetEventObject(), wxTextCtrl);
	if (!ctrl) return;

	wxPanel* const panel = static_cast<wxPanel*>(ctrl->GetClientData());
	const int index = Super::m_listMesh->FindPage(panel);
	if (index == wxNOT_FOUND)  //
		return;

	const wxString text = ctrl->GetValue();

	if (m_Model->StaticModelCast() != nullptr)
	{
		m_Model->StaticModelCast()->meshes[index].name = text;
	}
	else
	{
		m_Model->SkinnedModelCast()->meshes[index].name = text;
	}
	mark_changes();
}

void MainWindow::_on_texture_path_text_update(wxCommandEvent& event)
{
	wxTextCtrl* const ctrl = wxDynamicCast(event.GetEventObject(), wxTextCtrl);
	if (!ctrl) return;

	wxPanel* const panel = static_cast<wxPanel*>(ctrl->GetClientData());
	const int index = Super::m_listMesh->FindPage(panel);
	if (index == wxNOT_FOUND)  //
		return;

	const wxString text = ctrl->GetValue();

	if (m_Model->StaticModelCast() != nullptr)
	{
		m_Model->StaticModelCast()->meshes[index].texturesPath = text;
	}
	else
	{
		m_Model->SkinnedModelCast()->meshes[index].texturesPath = text;
	}
	mark_changes();
}

void MainWindow::_fill_as_static()
{
	const cxmf::StaticModel& model = *m_Model->StaticModelCast();

	Super::m_textCtrlModelName->SetValue(model.name);
	Super::m_TextSkinned->SetLabelText("OFF");

	wxListbook* const meshList = Super::m_listMesh;
	meshList->DeleteAllPages();

	Super::m_listBones->DeleteAllPages();

	size_t totalVerticies = 0;
	size_t totalFaces = 0;
	const std::string pagePrefix = "Mesh ";
	std::string pageName;
	const uint32_t meshesCount = static_cast<uint32_t>(model.meshes.size());
	for (uint32_t i_mesh = 0; i_mesh < meshesCount; ++i_mesh)
	{
		pageName = pagePrefix + std::to_string(i_mesh);

		const cxmf::Mesh& mesh = model.meshes[i_mesh];
		const size_t vertexCount = mesh.vertices.size();
		const size_t faceCount = mesh.indices.size() / 3;
		totalVerticies += vertexCount;
		totalFaces += faceCount;

		wxPanel* const mainPanel = new wxPanel(meshList);
		wxBoxSizer* const mainSizer = new wxBoxSizer(wxVERTICAL);

		_add_mesh_name_field(mainPanel, mainSizer, mesh.name);
		_add_texture_path_field(mainPanel, mainSizer, mesh.texturesPath);
		_add_vertex_counter(mainPanel, mainSizer, vertexCount);
		_add_face_counter(mainPanel, mainSizer, faceCount);

		mainPanel->SetSizer(mainSizer);
		mainPanel->Layout();
		mainSizer->Fit(mainPanel);
		meshList->AddPage(mainPanel, pageName);
	}

	set_total_verticies(totalVerticies);
	set_total_faces(totalFaces);
}

void MainWindow::_fill_as_skinned()
{
	const cxmf::SkinnedModel& model = *m_Model->SkinnedModelCast();

	Super::m_textCtrlModelName->SetValue(model.name);
	Super::m_TextSkinned->SetLabelText("ON");

	wxListbook* const meshList = Super::m_listMesh;
	meshList->DeleteAllPages();

	wxListbook* const boneList = Super::m_listBones;
	boneList->DeleteAllPages();

	size_t totalVerticies = 0;
	size_t totalFaces = 0;
	const std::string pagePrefix = "Mesh ";
	std::string pageName;
	const uint32_t meshesCount = static_cast<uint32_t>(model.meshes.size());
	for (uint32_t i_mesh = 0; i_mesh < meshesCount; ++i_mesh)
	{
		pageName = pagePrefix + std::to_string(i_mesh);

		const cxmf::SkinnedMesh& mesh = model.meshes[i_mesh];
		const size_t vertexCount = mesh.vertices.size();
		const size_t faceCount = mesh.indices.size() / 3;
		totalVerticies += vertexCount;
		totalFaces += faceCount;

		wxPanel* const mainPanel = new wxPanel(meshList);
		wxBoxSizer* const mainSizer = new wxBoxSizer(wxVERTICAL);

		_add_mesh_name_field(mainPanel, mainSizer, mesh.name);
		_add_texture_path_field(mainPanel, mainSizer, mesh.texturesPath);
		_add_vertex_counter(mainPanel, mainSizer, vertexCount);
		_add_face_counter(mainPanel, mainSizer, faceCount);

		mainPanel->SetSizer(mainSizer);
		mainPanel->Layout();
		mainSizer->Fit(mainPanel);
		meshList->AddPage(mainPanel, pageName);
	}

	set_total_verticies(totalVerticies);
	set_total_faces(totalFaces);
}

void MainWindow::set_total_verticies(size_t count)
{
	const std::string str = std::to_string(count);
	Super::m_TextTotalVerts->SetLabelText(str);
}

void MainWindow::set_total_faces(size_t count)
{
	const std::string str = std::to_string(count);
	Super::m_TextTotalFaces->SetLabelText(str);
}

void MainWindow::set_status_text(const std::string& text)
{
	Super::m_statusBar->SetStatusText(text);
}

void MainWindow::Reset()
{
	_show_controls(m_Model != nullptr);
	m_PreviewWnd->SetModel(m_Model);
	if (m_Model)
	{
		if (m_Model->StaticModelCast() != nullptr)
		{
			_fill_as_static();
		}
		else
		{
			_fill_as_skinned();
		}
	}
	unmark_changes();
}

void MainWindow::SetModel(cxmf::IModel* model)
{
	_destroy_model();
	m_Model = model;
	Reset();
}

void MainWindow::onWindowButton_Close(wxCloseEvent& event)
{
	if (m_Model && event.CanVeto())
	{
		StartClosing();
	}
	else
	{
		event.Skip();
	}
}

void MainWindow::onMenuSelect_File_Import(wxCommandEvent& /* event */)
{
	StartModelImport();
}

void MainWindow::onMenuSelect_File_Open(wxCommandEvent& /* event */)
{
	StartModelOpen();
}

void MainWindow::onMenuSelect_File_Save(wxCommandEvent& /* event */)
{
	StartModelSave();
}

void MainWindow::onMenuSelect_File_Exit(wxCommandEvent& /* event */)
{
	StartClosing();
}

void MainWindow::onMenuSelect_Info_Preview(wxCommandEvent& event)
{
	std::string msg;
	msg += "[LMB] - Rotate camera\n";
	msg += "[LMB+SHIFT] - Move camera\n";
	msg += "[SCROLL] - Zoom\n";
	msg += "[SCROLL+SHIFT] - Fast zoom\n";
	msg += "[R] - Reset camera\n";
	msg += "[Z] - ON/OFF wireframe\n";
	msg += "[X] - ON/OFF grid\n";
	msg += "[C] - ON/OFF light\n";
	wxMessageBox(msg, "Viewport controls", wxOK | wxCENTRE | wxICON_INFORMATION, this);
}

void MainWindow::onMenuSelect_Info_About(wxCommandEvent& /* event */)
{
	const std::string msg = std::format("CXMF ver. {}\nBuild date: {} {}\n", CXMF_VERSION_STRING, __DATE__, __TIME__);
	wxMessageBox(msg, "About", wxOK | wxCENTRE | wxICON_INFORMATION, this);
}

void MainWindow::onMenuSelect_Info_Github(wxCommandEvent& /* event */)
{
	wxLaunchDefaultBrowser("https://github.com/Orleonn/CXMFEditor");
}

void MainWindow::StartModelImport()
{
	wxFileDialog dialog(this, "Import model", wxEmptyString, wxEmptyString,	 //
						"(*.gltf;*.fbx)|*.gltf;*.fbx",						 //
						wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() == wxID_CANCEL)	//
		return;

	CXMFImportDialogWindow settingsDlg(this);
	if (settingsDlg.ShowModal() == wxID_CANCEL)	 //
		return;

	std::string filePath;
	{
		const wxString path = dialog.GetPath();
		filePath = path;
	}

	cxmf::ImportSettings importInfo;
	importInfo.angleGSN = settingsDlg.GetGSNAngle();
	importInfo.enableGSN = settingsDlg.GSNEnabled();
	importInfo.forceStatic = settingsDlg.ForceStaticEnabled();

	std::string err;
	cxmf::IModel* const newModel = cxmf::ImportModel(filePath, importInfo, err);
	if (!newModel)
	{
		if (err.empty()) err = "Unknown error!";
		wxMessageBox(err, "Error", wxOK | wxCENTRE | wxICON_ERROR, this);
		return;
	}

	if (!err.empty())
	{
		wxMessageBox(err, "Warning", wxOK | wxCENTRE | wxICON_WARNING, this);
	}

	SetModel(newModel);
	set_status_text(filePath);
	mark_changes();
}

void MainWindow::StartModelOpen()
{
	wxFileDialog dialog(this, "Open model", wxEmptyString, wxEmptyString,  //
						"CXM Format (*.cxm)|*.cxm",						   //
						wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() == wxID_CANCEL)	//
		return;

	std::string filePath;
	{
		const wxString path = dialog.GetPath();
		filePath = path;
	}

	std::string err;
	cxmf::IModel* const newModel = cxmf::OpenModel(filePath, err);
	if (!newModel)
	{
		if (err.empty()) err = "Unknown error!";
		wxMessageBox(err, "Error", wxOK | wxCENTRE | wxICON_ERROR, this);
		return;
	}

	if (!err.empty())
	{
		wxMessageBox(err, "Warning", wxOK | wxCENTRE | wxICON_WARNING, this);
	}

	SetModel(newModel);
	set_status_text(filePath);
}

void MainWindow::StartModelSave()
{
	if (!m_Model) return;

	std::string filePath;
	wxFileDialog dialog(this, "Save model", wxEmptyString, wxEmptyString,  //
						"CXM Format (*.cxm)|*.cxm",						   //
						wxFD_SAVE);
	for (;;)
	{
		if (dialog.ShowModal() == wxID_CANCEL)	//
			return;

		const wxString path = dialog.GetPath();
		filePath = path;
		if (std::filesystem::exists(filePath))
		{
			const char* const msg = "This file already exists.\nDo you want to overwrite it?";
			const int res = wxMessageBox(msg, "Warning", wxYES_NO | wxCENTRE | wxICON_WARNING, this);
			if (res == wxYES)  //
				break;
		}
		else
		{
			break;
		}
	}

	std::string err;
	if (cxmf::SaveModel(filePath, *m_Model, err))
	{
		wxMessageBox("Saved!", "Information", wxOK | wxCENTRE | wxICON_INFORMATION, this);
		set_status_text(filePath);
	}
	else
	{
		if (err.empty()) err = "Unknown error!";
		wxMessageBox(err, "Error", wxOK | wxCENTRE | wxICON_ERROR, this);
	}
	unmark_changes();
}

void MainWindow::StartClosing()
{
	if (m_Model && has_changes())
	{
		const char* const msg = "You have an unsaved model.\nDo you want to save it?";
		const int result = wxMessageBox(msg, "Warning", wxYES_NO | wxCANCEL | wxCENTRE | wxICON_WARNING, this);
		if (result == wxCANCEL)	 //
			return;

		if (result == wxYES)  //
			StartModelSave();
	}

	Close(true);
}



class MainApp final : public wxApp
{
public:
	bool OnInit() override;
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
	SetTopWindow(new MainWindow());
	return true;
}
