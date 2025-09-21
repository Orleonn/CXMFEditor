#pragma once

#include "CXMF.hpp"

#include <wx/glcanvas.h>



class PreviewWindow final : public wxGLCanvas
{
private:
	using Super = wxGLCanvas;

private:
	struct impl;

private:
	wxGLContext* m_GLCtx;
	impl* m_Impl;

private:
	void onPaintCallback(wxPaintEvent& event);
	void onMouseWheelCallback(wxMouseEvent& event);
	void onMouseLeftClickCallback(wxMouseEvent& event);
	void onKeyCallback(wxKeyEvent& event);
	void onUpdateCallback(wxUpdateUIEvent& event);
	void onFocusCallback(wxFocusEvent& event);

private:
	impl& get_impl() const;

	void draw_floor_grid();
	void draw_meshes();
	void draw_wireframe();
	void draw_center_axis();

public:
	PreviewWindow() = delete;

	explicit PreviewWindow(wxWindow* parent);

	~PreviewWindow();

public:
	void SetModel(const cxmf::IModel* model);
};
