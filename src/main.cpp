
#ifdef _MSC_VER
	#include "resource.h"
#endif

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "wx/clipbrd.h"
#include "wx/choicebk.h"
#include "CXMFEditor.h"

#include "CXMF.hpp"
#include "ModelPreview.hpp"

#include <filesystem>
#include <format>



class CXMFImportDialog final : public CXMFImportDlgBase
{
private:
	using Super = CXMFImportDlgBase;

private:
	void onButton_Ok(wxCommandEvent& event) override;

public:
	CXMFImportDialog() = delete;

	explicit CXMFImportDialog(wxWindow* parent);

	~CXMFImportDialog() = default;

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

CXMFImportDialog::CXMFImportDialog(wxWindow* parent)
	: Super(parent)
{
	//
}

void CXMFImportDialog::onButton_Ok(wxCommandEvent& /* event */)
{
	this->EndDialog(wxID_OK);
}



class CXMFTextDialog final : public CXMFTextDialogBase
{
private:
	using Super = CXMFTextDialogBase;

private:
	void OnButton_Ok(wxCommandEvent& event) override;
	void OnButton_Cancel(wxCommandEvent& event) override;

public:
	CXMFTextDialog() = delete;

	explicit CXMFTextDialog(wxWindow* parent, const wxString& title, const wxString& capText = wxEmptyString);

	~CXMFTextDialog() = default;

public:
	void SetCapText(const wxString& text);
	void SetCtrlText(const wxString& text);
	wxString GetCtrlText() const;
};

CXMFTextDialog::CXMFTextDialog(wxWindow* parent, const wxString& title, const wxString& capText)
	: Super(parent, wxID_ANY, title)
{
	if (!capText.empty()) Super::m_TextCap->SetLabelText(capText);
}

void CXMFTextDialog::OnButton_Ok(wxCommandEvent& /* event */)
{
	this->EndDialog(wxID_OK);
}

void CXMFTextDialog::OnButton_Cancel(wxCommandEvent& /* event */)
{
	this->EndDialog(wxID_CANCEL);
}

void CXMFTextDialog::SetCapText(const wxString& text)
{
	Super::m_TextCap->SetLabelText(text);
}

void CXMFTextDialog::SetCtrlText(const wxString& text)
{
	Super::m_textCtrl->SetValue(text);
}

wxString CXMFTextDialog::GetCtrlText() const
{
	return Super::m_textCtrl->GetValue();
}



class CXMFDumpDlg final : public CXMFDumpDlgBase
{
private:
	using Super = CXMFDumpDlgBase;

private:
	void OnButton_Ok(wxCommandEvent& event) override;
	void OnButton_Copy(wxCommandEvent& event) override;

public:
	CXMFDumpDlg() = delete;

	explicit CXMFDumpDlg(wxWindow* parent);

	~CXMFDumpDlg() = default;

public:
	void DumpModel(const cxmf::IModel& iModel);
	void DumpSkAnim(const cxmf::SkeletalAnimationArray& animations);
};

CXMFDumpDlg::CXMFDumpDlg(wxWindow* parent)
	: Super(parent)
{
	//
}

void CXMFDumpDlg::OnButton_Ok(wxCommandEvent& event)
{
	this->EndDialog(wxID_OK);
}

void CXMFDumpDlg::OnButton_Copy(wxCommandEvent& /* event */)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->Clear();

		wxTextDataObject* const data = new wxTextDataObject(Super::m_textCtrl->GetValue());
		if (wxTheClipboard->SetData(data))
		{
			wxMessageBox("Copied!", "Success", wxOK | wxCENTRE | wxICON_INFORMATION, this);
		}
		wxTheClipboard->Close();
	}
}

void CXMFDumpDlg::DumpModel(const cxmf::IModel& iModel)
{
	Super::m_textCtrl->SetValue(cxmf::DumpModel(iModel));
	ShowModal();
}

void CXMFDumpDlg::DumpSkAnim(const cxmf::SkeletalAnimationArray& animations)
{
	Super::m_textCtrl->SetValue(cxmf::DumpSkeletalAnimations(animations));
	ShowModal();
}



class SkeletonAnimNodeUserData final : public wxObject
{
public:
	wxChoicebook* nodeList;
	wxPanel* nodePanel;
	cxmf::AnimNode* node;
	cxmf::SkeletalAnimation* animation;
};



class MainWindow final : public CXMFWindowBase
{
private:
	using Super = CXMFWindowBase;

private:
	enum class object_type : uint32_t
	{
		model,
		sk_anim
	};

private:
	PreviewWindow* m_PreviewWnd;
	cxmf::IModel* m_Model;
	cxmf::SkeletalAnimationArray* m_SkAnims;
	bool m_HasChanges;

private:
	void _destroy_objects();
	void _show_controls();

	void _add_mesh_name_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName);
	void _add_texture_path_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName);
	void _add_vertex_counter(wxPanel* panel, wxBoxSizer* sizer, size_t vertexCount);
	void _add_face_counter(wxPanel* panel, wxBoxSizer* sizer, size_t faceCount);
	void _add_bone_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName, uint32_t parentIndex);
	void _add_skeleton_anim_node(wxPanel* panel, wxBoxSizer* sizer, wxChoicebook* nodeList,	 //
								 cxmf::AnimNode& node, cxmf::SkeletalAnimation& animation);
	void _add_skeleton_animation(wxPanel* panel, wxBoxSizer* sizer, cxmf::SkeletalAnimation& animation);

	void _on_mesh_name_text_update(wxCommandEvent& event);
	void _on_texture_path_text_update(wxCommandEvent& event);
	void _on_bone_name_change(wxCommandEvent& event);
	void _on_animation_name_change(wxCommandEvent& event);
	void _on_animation_node_name_change(wxCommandEvent& event);

	void _fill_as_static();
	void _fill_as_skinned();
	void _fill_as_sk_anim();

	void set_total_verticies(size_t count);
	void set_total_faces(size_t count);
	void set_status_text(const std::string& text);
	bool try_save_changes();

	void clear_main_panel()
	{
		const size_t count = Super::m_MainNotebook->GetPageCount();
		for (size_t i = 0; i < count; ++i)
		{
			Super::m_MainNotebook->RemovePage(0);
		}
	}

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
	void onMenuSelect_File_ImportModel(wxCommandEvent& event) override;
	void onMenuSelect_File_ImportSkAnim(wxCommandEvent& event) override;
	void onMenuSelect_File_OpenModel(wxCommandEvent& event) override;
	void onMenuSelect_File_OpenSkAnim(wxCommandEvent& event) override;
	void onMenuSelect_File_Save(wxCommandEvent& event) override;
	void onMenuSelect_File_Exit(wxCommandEvent& event) override;
	void onMenuSelect_Info_Preview(wxCommandEvent& event) override;
	void onMenuSelect_Info_About(wxCommandEvent& event) override;
	void onMenuSelect_Info_Github(wxCommandEvent& event) override;
	void OnModelNameChange(wxCommandEvent& event) override;
	void OnButton_DumpModel(wxCommandEvent& event) override;
	void OnButton_DumpSkAnim(wxCommandEvent& event) override;

private:
	void StartImporting(object_type type);
	void StartOpening(object_type type);
	void StartSaving();
	void StartClosing();

	void Reset();
	void SetModel(cxmf::IModel* model);
	void SetSkAnim(cxmf::SkeletalAnimationArray* animations);

	bool HasAnyObject() const
	{
		return (m_Model != nullptr || m_SkAnims != nullptr);
	}

public:
	MainWindow();
	~MainWindow();
};

MainWindow::MainWindow()
	: Super(nullptr), m_PreviewWnd(nullptr), m_Model(nullptr), m_SkAnims(nullptr), m_HasChanges(false)
{
#ifdef _MSC_VER
	const HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
	if (hIcon)
	{
		wxIcon icon;
		if (icon.CreateFromHICON(hIcon))
		{
			Super::SetIcon(icon);
		}
	}
#endif

	m_PreviewWnd = new PreviewWindow(Super::m_MainNotebook);
	clear_main_panel();

	this->Show(true);
}

MainWindow::~MainWindow()
{
	_destroy_objects();
}

void MainWindow::_destroy_objects()
{
	if (m_Model)
	{
		delete m_Model;
		m_Model = nullptr;
	}
	if (m_SkAnims)
	{
		delete m_SkAnims;
		m_SkAnims = nullptr;
	}
}

void MainWindow::_show_controls()
{
	const bool hasObject = HasAnyObject();

	wxMenuItem* const item = Super::m_menuFile->GetMenuItems()[2];
	if (item) item->Enable(hasObject);

	clear_main_panel();
	if (hasObject)
	{
		wxNotebook* const notebook = Super::m_MainNotebook;
		if (m_Model)
		{
			notebook->AddPage(Super::m_MainPanel_Model, "Model", true);
			notebook->AddPage(m_PreviewWnd, "Viewport", false);
		}
		else if (m_SkAnims)
		{
			notebook->AddPage(Super::m_MainPanel_SkAnim, "Animations", true);
		}
	}
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

void MainWindow::_add_bone_field(wxPanel* panel, wxBoxSizer* sizer, const std::string& baseName, uint32_t parentIndex)
{
	std::string parentNameText;
	if (parentIndex == cxmf::NO_PARENT)
		parentNameText = "NO_PARENT";
	else
		parentNameText = "Bone " + std::to_string(parentIndex);

	// Bone name
	wxBoxSizer* const nameCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const nameCapText = new wxStaticText(panel, wxID_ANY, "Name:");
	nameCapText->Wrap(-1);
	nameCapSizer->Add(nameCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* const nameText = new wxStaticText(panel, 0, baseName);
	nameCapSizer->Add(nameText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	// Parent index name
	wxBoxSizer* const parentCapSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* const parentCapText = new wxStaticText(panel, wxID_ANY, "Parent:");
	parentCapText->Wrap(-1);
	parentCapSizer->Add(parentCapText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* const parentText = new wxStaticText(panel, wxID_ANY, parentNameText);
	parentCapSizer->Add(parentText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	// Name changer button
	wxBoxSizer* const nameChangeBtnSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* const nameChangeBtn = new wxButton(panel, wxID_ANY, "Change bone name");
	nameChangeBtnSizer->Add(nameChangeBtn, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	sizer->Add(nameCapSizer, 0, wxEXPAND, 5);
	sizer->Add(parentCapSizer, 0, wxEXPAND, 5);
	sizer->Add(nameChangeBtnSizer, 0, wxEXPAND, 5);

	nameChangeBtn->SetClientData(panel);
	nameChangeBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::_on_bone_name_change),  //
						   nullptr, this);
}

void MainWindow::_add_skeleton_anim_node(wxPanel* panel, wxBoxSizer* sizer, wxChoicebook* nodeList,	 //
										 cxmf::AnimNode& node, cxmf::SkeletalAnimation& animation)
{
	const std::string posStr = std::format("Position keys: {}", node.positionKeys.size());
	const std::string rotStr = std::format("Rotation keys: {}", node.rotationKeys.size());
	const std::string sclStr = std::format("Scale keys: {}", node.scaleKeys.size());

	wxButton* const changeNameBtn = new wxButton(panel, wxID_ANY, "Change name");
	sizer->Add(changeNameBtn, 0, wxALL, 5);

	wxStaticText* const posText = new wxStaticText(panel, wxID_ANY, posStr);
	posText->Wrap(-1);
	sizer->Add(posText, 0, wxALL, 5);

	wxStaticText* const rotText = new wxStaticText(panel, wxID_ANY, rotStr);
	rotText->Wrap(-1);
	sizer->Add(rotText, 0, wxALL, 5);

	wxStaticText* const sclText = new wxStaticText(panel, wxID_ANY, sclStr);
	sclText->Wrap(-1);
	sizer->Add(sclText, 0, wxALL, 5);

	SkeletonAnimNodeUserData* const userData = new SkeletonAnimNodeUserData();
	userData->nodeList = nodeList;
	userData->nodePanel = panel;
	userData->node = &node;
	userData->animation = &animation;

	changeNameBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::_on_animation_node_name_change),	 //
						   userData, this);
}

void MainWindow::_add_skeleton_animation(wxPanel* panel, wxBoxSizer* sizer, cxmf::SkeletalAnimation& animation)
{
	const std::string durationStr = std::format("Duration: {:.3f} ({:.3f} sec)",  //
												animation.duration, (animation.duration / animation.ticksPerSecond));
	const std::string ticksPerSecStr = std::format("Ticks per second: {}",		  //
												   animation.ticksPerSecond);

	wxStaticText* const nameText = new wxStaticText(panel, 0, animation.name);
	nameText->Wrap(-1);
	sizer->Add(nameText, 0, wxALL, 5);

	wxButton* const changeNameBtn = new wxButton(panel, wxID_ANY, "Change name");
	sizer->Add(changeNameBtn, 0, wxALL, 5);

	wxStaticText* const durText = new wxStaticText(panel, wxID_ANY, durationStr);
	durText->Wrap(-1);
	sizer->Add(durText, 0, wxALL, 5);

	wxStaticText* const tpsText = new wxStaticText(panel, wxID_ANY, ticksPerSecStr);
	tpsText->Wrap(-1);
	sizer->Add(tpsText, 0, wxALL, 5);

	wxStaticLine* const divider1 = new wxStaticLine(panel);
	sizer->Add(divider1, 0, wxEXPAND | wxALL, 5);

	wxChoicebook* const nodeList = new wxChoicebook(panel, wxID_ANY);
	for (cxmf::AnimNode& node : animation.nodes)
	{
		wxPanel* const nodePanel = new wxPanel(nodeList);
		wxBoxSizer* const nodeSizer = new wxBoxSizer(wxVERTICAL);

		_add_skeleton_anim_node(nodePanel, nodeSizer, nodeList, node, animation);

		nodePanel->SetSizer(nodeSizer);
		nodePanel->Layout();
		nodeSizer->Fit(nodePanel);
		nodeList->AddPage(nodePanel, node.boneName);
	}
	sizer->Add(nodeList, 1, wxEXPAND | wxALL, 5);

	changeNameBtn->SetClientData(panel);
	changeNameBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::_on_animation_name_change),	//
						   nullptr, this);
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

void MainWindow::_on_bone_name_change(wxCommandEvent& event)
{
	wxButton* const btn = wxDynamicCast(event.GetEventObject(), wxButton);
	if (!btn) return;

	wxPanel* const panel = static_cast<wxPanel*>(btn->GetClientData());
	const int index = Super::m_listBones->FindPage(panel);
	if (index == wxNOT_FOUND)  //
		return;

	cxmf::SkinnedModel& model = *m_Model->SkinnedModelCast();
	cxmf::Bone& bone = model.bones[index];

	wxStaticText* const panelBoneName = wxDynamicCast(panel->FindItem(0), wxStaticText);
	if (!panelBoneName)	 //
		return;

	CXMFTextDialog dialog(this, btn->GetLabelText(), "Enter new name");
	for (;;)
	{
		dialog.SetCtrlText(bone.name);
		if (dialog.ShowModal() == wxID_CANCEL)	//
			break;

		std::string newName;
		{
			const wxString tmpName = dialog.GetCtrlText();
			newName = tmpName.c_str();
		}

		if (newName == bone.name)  //
			break;

		if (newName.empty())
		{
			wxMessageBox("The name must not be empty!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		if (model.boneMapping.contains(newName))
		{
			wxMessageBox("Bone with this name already exists!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		model.boneMapping.erase(bone.name);
		model.boneMapping[newName] = bone.id;
		bone.name = newName;
		panelBoneName->SetLabelText(newName);
		mark_changes();
		break;
	}
}

void MainWindow::_on_animation_name_change(wxCommandEvent& event)
{
	wxButton* const btn = wxDynamicCast(event.GetEventObject(), wxButton);
	if (!btn) return;

	wxPanel* const panel = static_cast<wxPanel*>(btn->GetClientData());
	const int index = Super::m_listSkAnims->FindPage(panel);
	if (index == wxNOT_FOUND)  //
		return;

	cxmf::SkeletalAnimation& animation = (*m_SkAnims)[index];

	CXMFTextDialog dialog(this, btn->GetLabelText(), "Enter new name");
	for (;;)
	{
		dialog.SetCtrlText(animation.name);
		if (dialog.ShowModal() == wxID_CANCEL)	//
			break;

		std::string newName;
		{
			const wxString tmpName = dialog.GetCtrlText();
			newName = tmpName.c_str();
		}

		if (newName == animation.name)	//
			break;

		if (newName.empty())
		{
			wxMessageBox("The name must not be empty!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		bool nameExist = false;
		for (const cxmf::SkeletalAnimation& anim : *m_SkAnims)
		{
			if (newName == anim.name)
			{
				nameExist = true;
				break;
			}
		}

		if (nameExist)
		{
			wxMessageBox("Animation with this name already exists!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		animation.name = newName;
		Super::m_listSkAnims->SetPageText(index, newName);
		wxStaticText* const nameText = wxDynamicCast(panel->FindItem(0), wxStaticText);
		if (nameText) nameText->SetLabelText(newName);
		mark_changes();
		break;
	}
}

void MainWindow::_on_animation_node_name_change(wxCommandEvent& event)
{
	wxButton* const btn = wxDynamicCast(event.GetEventObject(), wxButton);
	if (!btn) return;

	SkeletonAnimNodeUserData* const userData = static_cast<SkeletonAnimNodeUserData*>(event.GetEventUserData());
	const int index = userData->nodeList->FindPage(userData->nodePanel);
	if (index == wxNOT_FOUND)  //
		return;

	CXMFTextDialog dialog(this, btn->GetLabelText(), "Enter new name");
	for (;;)
	{
		dialog.SetCtrlText(userData->node->boneName);
		if (dialog.ShowModal() == wxID_CANCEL)	//
			break;

		std::string newName;
		{
			const wxString tmpName = dialog.GetCtrlText();
			newName = tmpName.c_str();
		}

		if (newName == userData->node->boneName)  //
			break;

		if (newName.empty())
		{
			wxMessageBox("The name must not be empty!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		bool nameExist = false;
		for (const cxmf::AnimNode& node : userData->animation->nodes)
		{
			if (newName == node.boneName)
			{
				nameExist = true;
				break;
			}
		}

		if (nameExist)
		{
			wxMessageBox("Node with this name already exists!", "Error", wxOK | wxCENTRE | wxICON_ERROR, &dialog);
			continue;
		}

		userData->node->boneName = newName;
		userData->nodeList->SetPageText(index, newName);
		mark_changes();
		break;
	}
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
	std::string pagePrefix = "Mesh ";
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

	pagePrefix = "Bone ";
	const uint32_t bonesCount = static_cast<uint32_t>(model.bones.size());
	for (uint32_t i_bone = 0; i_bone < bonesCount; ++i_bone)
	{
		pageName = pagePrefix + std::to_string(i_bone);
		const cxmf::Bone& node = model.bones[i_bone];

		wxPanel* const mainPanel = new wxPanel(boneList);
		wxBoxSizer* const mainSizer = new wxBoxSizer(wxVERTICAL);

		_add_bone_field(mainPanel, mainSizer, node.name, node.parentIndex);

		mainPanel->SetSizer(mainSizer);
		mainPanel->Layout();
		mainSizer->Fit(mainPanel);
		boneList->AddPage(mainPanel, pageName);
	}

	set_total_verticies(totalVerticies);
	set_total_faces(totalFaces);
}

void MainWindow::_fill_as_sk_anim()
{
	wxListbook* const listAnims = Super::m_listSkAnims;
	listAnims->DeleteAllPages();

	for (cxmf::SkeletalAnimation& animation : *m_SkAnims)
	{
		wxPanel* const animPanel = new wxPanel(listAnims);
		wxBoxSizer* const animSizer = new wxBoxSizer(wxVERTICAL);

		_add_skeleton_animation(animPanel, animSizer, animation);

		animPanel->SetSizer(animSizer);
		animPanel->Layout();
		animSizer->Fit(animPanel);
		listAnims->AddPage(animPanel, animation.name);
	}
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

bool MainWindow::try_save_changes()
{
	if (HasAnyObject() && has_changes())
	{
		const char* const msg = "You have an unsaved data.\nDo you want to save it?";
		const int result = wxMessageBox(msg, "Warning", wxYES_NO | wxCANCEL | wxCENTRE | wxICON_WARNING, this);
		if (result == wxCANCEL)	 //
			return false;

		if (result == wxYES)  //
			StartSaving();
	}
	return true;
}

void MainWindow::Reset()
{
	if (m_Model)
	{
		m_PreviewWnd->SetModel(m_Model);
		if (m_Model->StaticModelCast() != nullptr)
		{
			_fill_as_static();
		}
		else
		{
			_fill_as_skinned();
		}
	}
	else
	{
		m_PreviewWnd->SetModel(nullptr);
		if (m_SkAnims)
		{
			_fill_as_sk_anim();
		}
	}

	_show_controls();
	unmark_changes();
}

void MainWindow::SetModel(cxmf::IModel* model)
{
	_destroy_objects();
	m_Model = model;
	Reset();
}

void MainWindow::SetSkAnim(cxmf::SkeletalAnimationArray* animations)
{
	_destroy_objects();
	m_SkAnims = animations;
	Reset();
}

void MainWindow::onWindowButton_Close(wxCloseEvent& event)
{
	if (HasAnyObject() && event.CanVeto())
	{
		StartClosing();
	}
	else
	{
		event.Skip();
	}
}

void MainWindow::onMenuSelect_File_ImportModel(wxCommandEvent& /* event */)
{
	StartImporting(object_type::model);
}

void MainWindow::onMenuSelect_File_ImportSkAnim(wxCommandEvent& /* event */)
{
	StartImporting(object_type::sk_anim);
}

void MainWindow::onMenuSelect_File_OpenModel(wxCommandEvent& /* event */)
{
	StartOpening(object_type::model);
}

void MainWindow::onMenuSelect_File_OpenSkAnim(wxCommandEvent& /* event */)
{
	StartOpening(object_type::sk_anim);
}

void MainWindow::onMenuSelect_File_Save(wxCommandEvent& /* event */)
{
	StartSaving();
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

void MainWindow::OnModelNameChange(wxCommandEvent& event)
{
	if (!m_Model)
	{
		event.Skip();
		return;
	}

	std::string* name = nullptr;
	if (m_Model->StaticModelCast() != nullptr)
	{
		name = &m_Model->StaticModelCast()->name;
	}
	else
	{
		name = &m_Model->SkinnedModelCast()->name;
	}

	std::string newName;
	{
		const wxString tmpStr = Super::m_textCtrlModelName->GetValue();
		newName = tmpStr.c_str();
	}

	if (newName == *name)  //
		return;

	if (newName.empty())
	{
		Super::m_textCtrlModelName->ChangeValue(*name);
		Super::m_textCtrlModelName->SetInsertionPoint(-1);
	}
	else
	{
		*name = newName;
		mark_changes();
	}
}

void MainWindow::OnButton_DumpModel(wxCommandEvent& event)
{
	if (m_Model)
	{
		CXMFDumpDlg dialog(this);
		dialog.DumpModel(*m_Model);
	}
	else
	{
		event.Skip();
	}
}

void MainWindow::OnButton_DumpSkAnim(wxCommandEvent& event)
{
	if (m_SkAnims)
	{
		CXMFDumpDlg dialog(this);
		dialog.DumpSkAnim(*m_SkAnims);
	}
	else
	{
		event.Skip();
	}
}

void MainWindow::StartImporting(object_type type)
{
	if (!try_save_changes())  //
		return;

	std::string dlg_message;
	std::string dlg_wildCard;
	if (type == object_type::model)
	{
		dlg_message = "Import model";
		dlg_wildCard = "(*.gltf;*.fbx)|*.gltf;*.fbx";
	}
	else if (type == object_type::sk_anim)
	{
		dlg_message = "Import skeletal animations";
		dlg_wildCard = "(*.gltf;*.fbx)|*.gltf;*.fbx";
	}
	else
	{
		return;
	}

	wxFileDialog dialog(this, dlg_message, wxEmptyString, wxEmptyString, dlg_wildCard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL)	//
		return;

	std::string filePath;
	{
		const wxString path = dialog.GetPath();
		filePath = path;
	}

	std::string err;
	cxmf::IModel* newModel = nullptr;
	cxmf::SkeletalAnimationArray* newAnims = nullptr;

	if (type == object_type::model)
	{
		CXMFImportDialog settingsDlg(this);
		if (settingsDlg.ShowModal() == wxID_CANCEL)	 //
			return;

		cxmf::ImportSettings importInfo;
		importInfo.angleGSN = settingsDlg.GetGSNAngle();
		importInfo.enableGSN = settingsDlg.GSNEnabled();
		importInfo.forceStatic = settingsDlg.ForceStaticEnabled();

		newModel = cxmf::ImportModel(filePath, importInfo, err);
	}
	else if (type == object_type::sk_anim)
	{
		newAnims = cxmf::ImportSkeletalAnimations(filePath, err);
	}

	if (!newModel && !newAnims)
	{
		if (err.empty()) err = "Unknown error!";
		wxMessageBox(err, "Error", wxOK | wxCENTRE | wxICON_ERROR, this);
		return;
	}

	if (!err.empty())
	{
		wxMessageBox(err, "Warning", wxOK | wxCENTRE | wxICON_WARNING, this);
	}

	if (newModel)
		SetModel(newModel);
	else if (newAnims)
		SetSkAnim(newAnims);

	set_status_text(filePath);
	mark_changes();
}

void MainWindow::StartOpening(object_type type)
{
	if (!try_save_changes())  //
		return;

	std::string dlg_message;
	std::string dlg_wildCard;
	if (type == object_type::model)
	{
		dlg_message = "Open model";
		dlg_wildCard = "CXM Format (*.cxm)|*.cxm";
	}
	else if (type == object_type::sk_anim)
	{
		dlg_message = "Open skeletal animations";
		dlg_wildCard = "CXANM Format (*.cxanm)|*.cxanm";
	}
	else
	{
		return;
	}

	wxFileDialog dialog(this, dlg_message, wxEmptyString, wxEmptyString, dlg_wildCard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL)	//
		return;

	std::string filePath;
	{
		const wxString path = dialog.GetPath();
		filePath = path;
	}

	std::string err;
	cxmf::IModel* newModel = nullptr;
	cxmf::SkeletalAnimationArray* newAnims = nullptr;
	if (type == object_type::model)
	{
		newModel = cxmf::OpenModel(filePath, err);
	}
	else if (type == object_type::sk_anim)
	{
		newAnims = cxmf::OpenSkeletalAnimations(filePath, err);
	}

	if (!newModel && !newAnims)
	{
		if (err.empty()) err = "Unknown error!";
		wxMessageBox(err, "Error", wxOK | wxCENTRE | wxICON_ERROR, this);
		return;
	}

	if (!err.empty())
	{
		wxMessageBox(err, "Warning", wxOK | wxCENTRE | wxICON_WARNING, this);
	}

	if (newModel)
		SetModel(newModel);
	else if (newAnims)
		SetSkAnim(newAnims);

	set_status_text(filePath);
}

void MainWindow::StartSaving()
{
	if (!HasAnyObject()) return;

	std::string dlg_message;
	std::string dlg_wildCard;
	if (m_Model)
	{
		dlg_message = "Save model";
		dlg_wildCard = "CXM Format (*.cxm)|*.cxm";
	}
	else if (m_SkAnims)
	{
		dlg_message = "Save skeletal animations";
		dlg_wildCard = "CXANM Format (*.cxanm)|*.cxanm";
	}

	std::string filePath;
	wxFileDialog dialog(this, dlg_message, wxEmptyString, wxEmptyString, dlg_wildCard, wxFD_SAVE);
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
	bool saveResult = false;
	if (m_Model)
	{
		saveResult = cxmf::SaveModel(filePath, *m_Model, err);
	}
	else if (m_SkAnims)
	{
		saveResult = cxmf::SaveSkeletalAnimations(filePath, *m_SkAnims, err);
	}

	if (saveResult)
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
	if (try_save_changes())	 //
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
