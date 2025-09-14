#pragma once

#include "CXMF.hpp"

#include <wx/glcanvas.h>



class PreviewWindow final : public wxGLCanvas
{
private:
	using Super = wxGLCanvas;

private:
	wxGLContext* m_GLCtx;

private:
	void _init_shaders();

private:
	void onPaintCallback(wxPaintEvent& event);
	void onMouseWheelCallback(wxMouseEvent& event);
	void onMouseLeftClickCallback(wxMouseEvent& event);

public:
	PreviewWindow() = delete;

	explicit PreviewWindow(wxWindow* parent);

	~PreviewWindow();
};
