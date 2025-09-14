
#include "ModelPreview.hpp"



PreviewWindow::PreviewWindow(wxWindow* parent)
	: Super(parent, wxGLAttributes().Defaults()), m_GLCtx(nullptr)
{
	wxGLContextAttrs attribs;
	attribs.CoreProfile();
	attribs.OGLVersion(4, 6);
	m_GLCtx = new wxGLContext(this, nullptr, &attribs);
	if (!m_GLCtx->IsOK())
	{
		wxSafeShowMessage("Error", "Failed to initialize OpenGL context!");
		wxExit();
	}

	Connect(wxEVT_PAINT, wxPaintEventHandler(PreviewWindow::onPaintCallback), nullptr, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(PreviewWindow::onMouseWheelCallback), nullptr, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PreviewWindow::onMouseLeftClickCallback), nullptr, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PreviewWindow::onMouseLeftClickCallback), nullptr, this);
}

PreviewWindow::~PreviewWindow()
{
	delete m_GLCtx;
}

void PreviewWindow::_init_shaders()
{
	//
}

void PreviewWindow::onPaintCallback(wxPaintEvent& event)
{
	//
}

void PreviewWindow::onMouseWheelCallback(wxMouseEvent& event)
{
	//
}

void PreviewWindow::onMouseLeftClickCallback(wxMouseEvent& event)
{
	//
}
