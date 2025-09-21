#include "ModelPreview.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"



struct viewport_camera
{
	glm::mat4 projView;
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 target;
	float distance;
	float fovy;
	float aspect;
	float zNear;
	float zFar;

	viewport_camera()
		: projView(1.0F),
		  rotation(0.0F, 0.0F, 0.0F, 1.0F),
		  position(0.0F, 0.0F, 0.0F),
		  target(0.0F, 0.0F, 0.0F),
		  distance(5.0F),
		  fovy(glm::radians(65.0F)),
		  aspect(1366.0F / 768.0F),
		  zNear(0.1F),
		  zFar(1000.0F)
	{}

	glm::vec3 forward() const
	{
		return rotation * glm::vec3(0.0F, 0.0F, -1.0F);
	}

	glm::vec3 center() const
	{
		return position + forward();
	}

	glm::vec3 right() const
	{
		return rotation * glm::vec3(1.0F, 0.0F, 0.0F);
	}

	glm::vec3 up() const
	{
		return rotation * glm::vec3(0.0F, 1.0F, 0.0F);
	}

	void update()
	{
		position = target - forward() * distance;
		glm::mat4 proj = glm::perspective(fovy, aspect, zNear, zFar);
		proj[1][1] *= -1.0F;
		const glm::mat4 view = glm::lookAt(position, center(), up());
		projView = proj * view;
	}
};



enum camera_move_mode
{
	CAMERA_MODE_NONE,
	CAMERA_MODE_ROTATE,
	CAMERA_MODE_MOVE
};

enum draw_mesh_mode
{
	DRAW_MESH_MODE_MESHES,
	DRAW_MESH_MODE_WIREFRAME
};

struct PreviewWindow::impl
{
	viewport_camera cam;
	const cxmf::IModel* model;
	wxPoint lastCPos;
	camera_move_mode moveMode;
	draw_mesh_mode drawMeshMode;
	bool shiftIsPressed;
	bool allowFloorGrid;
	bool allowFlashlight;

	impl()
		: cam(),
		  model(nullptr),
		  lastCPos(),
		  moveMode(CAMERA_MODE_NONE),
		  drawMeshMode(DRAW_MESH_MODE_MESHES),
		  shiftIsPressed(false),
		  allowFloorGrid(true),
		  allowFlashlight(true)
	{}
};

static wxGLAttributes get_opengl_attributes()
{
	wxGLAttributes attrs;
	attrs.Defaults().EndList();
	return attrs;
}



PreviewWindow::PreviewWindow(wxWindow* parent)
	: Super(parent, get_opengl_attributes()), m_GLCtx(nullptr), m_Impl(nullptr)
{
	wxGLContext* const ctx = new wxGLContext(this);
	if (!ctx->IsOK())
	{
		delete ctx;
		wxSafeShowMessage("Error", "Failed to initialize OpenGL context!");
		wxExit();
		return;
	}

	m_GLCtx = ctx;
	m_Impl = new impl();

	Connect(wxEVT_PAINT, wxPaintEventHandler(PreviewWindow::onPaintCallback), nullptr, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(PreviewWindow::onMouseWheelCallback), nullptr, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PreviewWindow::onMouseLeftClickCallback), nullptr, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PreviewWindow::onMouseLeftClickCallback), nullptr, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(PreviewWindow::onKeyCallback), nullptr, this);
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(PreviewWindow::onKeyCallback), nullptr, this);
	Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(PreviewWindow::onUpdateCallback), nullptr, this);
	Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(PreviewWindow::onFocusCallback), nullptr, this);
}

PreviewWindow::~PreviewWindow()
{
	delete m_GLCtx;
	delete m_Impl;
}

void PreviewWindow::onPaintCallback(wxPaintEvent& /* event */)
{
	m_GLCtx->SetCurrent(*this);

	viewport_camera& camera = get_impl().cam;

	GLsizei viewW, viewH;
	this->GetSize(&viewW, &viewH);

	glViewport(0, 0, viewW, viewH);
	glClearColor(0.2471F, 0.2471F, 0.2471F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.aspect = static_cast<float>(viewW) / viewH;
	camera.update();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&camera.projView[0][0]);

	draw_floor_grid();
	draw_meshes();
	draw_center_axis();

	this->SwapBuffers();
}

void PreviewWindow::onMouseWheelCallback(wxMouseEvent& event)
{
	const int rotation = event.GetWheelRotation();
	if (rotation == 0) return;

	viewport_camera& camera = get_impl().cam;
	float zoomFactor = (rotation > 0) ? -0.25F : 0.25F;
	if (get_impl().shiftIsPressed)
	{
		zoomFactor *= 8.0F;
	}

	camera.distance += zoomFactor;
	this->Refresh();
}

void PreviewWindow::onMouseLeftClickCallback(wxMouseEvent& event)
{
	if (event.GetButton() != wxMOUSE_BTN_LEFT)
	{
		event.Skip();
		return;
	}

	impl& data = get_impl();
	if (event.ButtonDown())
	{
		data.lastCPos = wxGetMousePosition();
		this->CaptureMouse();

		data.moveMode = data.shiftIsPressed ? CAMERA_MODE_MOVE : CAMERA_MODE_ROTATE;
	}
	else
	{
		if (this->HasCapture())	 //
			this->ReleaseMouse();

		data.moveMode = CAMERA_MODE_NONE;
	}
}

void PreviewWindow::onKeyCallback(wxKeyEvent& event)
{
	const bool is_pressed = event.GetEventType() == wxEVT_KEY_DOWN;
	if (is_pressed && event.IsAutoRepeat())
	{
		event.Skip();
		return;
	}

	impl& data = get_impl();
	switch (event.GetKeyCode())
	{
		case 'R':
		{
			if (is_pressed)
			{
				data.cam = viewport_camera();
				this->Refresh();
			}
			break;
		}
		case 'Z':
		{
			if (is_pressed)
			{
				if (data.drawMeshMode == DRAW_MESH_MODE_MESHES)
					data.drawMeshMode = DRAW_MESH_MODE_WIREFRAME;
				else
					data.drawMeshMode = DRAW_MESH_MODE_MESHES;
				this->Refresh();
			}
			break;
		}
		case 'X':
		{
			if (is_pressed)
			{
				data.allowFloorGrid = !data.allowFloorGrid;
				this->Refresh();
			}
			break;
		}
		case 'C':
		{
			if (is_pressed)
			{
				data.allowFlashlight = !data.allowFlashlight;
				this->Refresh();
			}
			break;
		}
		case WXK_SHIFT:
		{
			data.shiftIsPressed = is_pressed;
			if (data.moveMode != CAMERA_MODE_NONE)
			{
				data.moveMode = data.shiftIsPressed ? CAMERA_MODE_MOVE : CAMERA_MODE_ROTATE;
			}
			break;
		}
		default:
		{
			event.Skip();
			break;
		}
	}
}

void PreviewWindow::onUpdateCallback(wxUpdateUIEvent& event)
{
	impl& data = get_impl();
	if (data.moveMode == CAMERA_MODE_NONE)
	{
		event.Skip();
		return;
	}

	const wxPoint newCPos = wxGetMousePosition();
	if (data.lastCPos == newCPos)
	{
		event.Skip();
		return;
	}

	const float sensitivity = 0.005F;

	const float deltaX = newCPos.x - data.lastCPos.x;
	const float deltaY = newCPos.y - data.lastCPos.y;

	if (data.moveMode == CAMERA_MODE_ROTATE)
	{
		const glm::quat rotX = glm::angleAxis(deltaY * sensitivity, glm::vec3(1.0F, 0.0F, 0.0F));
		const glm::quat rotY = glm::angleAxis(deltaX * sensitivity, glm::vec3(0.0F, 1.0F, 0.0F));
		const glm::quat newRot = rotY * data.cam.rotation * rotX;
		data.cam.rotation = glm::normalize(newRot);
	}
	else if (data.moveMode == CAMERA_MODE_MOVE)
	{
		data.cam.target -= data.cam.right() * deltaX * sensitivity;
		data.cam.target += data.cam.up() * -deltaY * sensitivity;
	}

	data.lastCPos = newCPos;
	this->Refresh();
}

void PreviewWindow::onFocusCallback(wxFocusEvent& event)
{
	impl& data = get_impl();
	data.shiftIsPressed = false;
	data.moveMode = CAMERA_MODE_NONE;
}

PreviewWindow::impl& PreviewWindow::get_impl() const
{
	return *m_Impl;
}

void PreviewWindow::draw_floor_grid()
{
	if (!get_impl().allowFloorGrid)	 //
		return;

	const float gridSize = 20.0F;
	const float gridStep = 0.5F;
	const float gridAlpha = 0.5F;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0F);
	glColor4f(0.3623F, 0.3623F, 0.3623F, gridAlpha);
	glBegin(GL_LINES);

	for (float i = -gridSize; i <= gridSize; i += gridStep)
	{
		if (i != 0.0F)
		{
			// Horizontal lines (X)
			glVertex3f(-gridSize, 0.0F, i);
			glVertex3f(gridSize, 0.0F, i);

			// Vertical lines (Z)
			glVertex3f(i, 0.0F, -gridSize);
			glVertex3f(i, 0.0F, gridSize);
		}
	}

	glEnd();

	// Colored axis lines

	glLineWidth(2.0F);
	glBegin(GL_LINES);

	// X
	glColor4f(0.9F, 0.0F, 0.0F, gridAlpha);
	glVertex3f(-gridSize, 0.0F, 0.0F);
	glVertex3f(gridSize, 0.0F, 0.0F);

	// Z
	glColor4f(0.0F, 0.0F, 0.9F, gridAlpha);
	glVertex3f(0.0F, 0.0F, -gridSize);
	glVertex3f(0.0F, 0.0F, gridSize);

	glEnd();

	glDisable(GL_BLEND);
	glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
}

void PreviewWindow::draw_meshes()
{
	impl& data = get_impl();
	if (data.drawMeshMode == DRAW_MESH_MODE_WIREFRAME)
	{
		draw_wireframe();
		return;
	}

	if (data.allowFlashlight)
	{
		const viewport_camera& camera = data.cam;
		const glm::vec4 lightPos(camera.position, 1.0F);
		const glm::vec3 lightDir = camera.forward();

		const glm::vec4 lightAmbient(0.25F, 0.25F, 0.25F, 1.0F);
		const glm::vec4 lightDiffuse(0.95F, 0.95F, 0.95F, 1.0F);
		const glm::vec4 lightSpecular(1.0F, 1.0F, 1.0F, 1.0F);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glLightfv(GL_LIGHT0, GL_POSITION, &lightPos[0]);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, &lightDir[0]);

		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0F);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0F);

		glLightfv(GL_LIGHT0, GL_AMBIENT, &lightAmbient[0]);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, &lightDiffuse[0]);
		glLightfv(GL_LIGHT0, GL_SPECULAR, &lightSpecular[0]);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0F);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0F);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0F);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.5F, 0.5F, 0.5F);
	glBegin(GL_TRIANGLES);

	const cxmf::IModel* const i_model = get_impl().model;
	if (i_model->StaticModelCast())
	{
		const cxmf::StaticModel& model = *i_model->StaticModelCast();
		for (const cxmf::Mesh& mesh : model.meshes)
		{
			const uint32_t maxIndices = static_cast<uint32_t>(mesh.indices.size());
			for (uint32_t index = 0; index < maxIndices; index += 3)
			{

				for (uint32_t i = 0; i < 3; ++i)
				{
					const uint32_t idx = mesh.indices[index + i];
					const cxmf::Vertex& vert = mesh.vertices[idx];
					glNormal3f(vert.normal[0], vert.normal[1], vert.normal[2]);
					glVertex3f(vert.position[0], vert.position[1], vert.position[2]);
				}
			}
		}
	}
	else
	{
		const cxmf::SkinnedModel& model = *i_model->SkinnedModelCast();
		for (const cxmf::SkinnedMesh& mesh : model.meshes)
		{
			const uint32_t maxIndices = static_cast<uint32_t>(mesh.indices.size());
			for (uint32_t index = 0; index < maxIndices; index += 3)
			{

				for (uint32_t i = 0; i < 3; ++i)
				{
					const uint32_t idx = mesh.indices[index + i];
					const cxmf::WeightedVertex& vert = mesh.vertices[idx];
					glNormal3f(vert.normal[0], vert.normal[1], vert.normal[2]);
					glVertex3f(vert.position[0], vert.position[1], vert.position[2]);
				}
			}
		}
	}

	glEnd();

	if (data.allowFlashlight)
	{
		glDisable(GL_LIGHTING);
	}
}

void PreviewWindow::draw_wireframe()
{
	const auto drawEdge = [](const float* vA, const float* vB) -> void
	{
		glVertex3f(vA[0], vA[1], vA[2]);
		glVertex3f(vB[0], vB[1], vB[2]);
	};

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(0.9608F, 0.6F, 0.1843F, 1.0F);
	glLineWidth(1.0F);
	glBegin(GL_LINES);

	const cxmf::IModel* const i_model = get_impl().model;
	if (i_model->StaticModelCast())
	{
		const cxmf::StaticModel& model = *i_model->StaticModelCast();
		for (const cxmf::Mesh& mesh : model.meshes)
		{
			const uint32_t maxIndices = static_cast<uint32_t>(mesh.indices.size());
			for (uint32_t index = 0; index < maxIndices; index += 3)
			{
				const uint32_t idx0 = mesh.indices[index];
				const uint32_t idx1 = mesh.indices[index + 1];
				const uint32_t idx2 = mesh.indices[index + 2];

				drawEdge(mesh.vertices[idx0].position, mesh.vertices[idx1].position);
				drawEdge(mesh.vertices[idx1].position, mesh.vertices[idx2].position);
				drawEdge(mesh.vertices[idx2].position, mesh.vertices[idx0].position);
			}
		}
	}
	else
	{
		const cxmf::SkinnedModel& model = *i_model->SkinnedModelCast();
		for (const cxmf::SkinnedMesh& mesh : model.meshes)
		{
			const uint32_t maxIndices = static_cast<uint32_t>(mesh.indices.size());
			for (uint32_t index = 0; index < maxIndices; index += 3)
			{
				const uint32_t idx0 = mesh.indices[index];
				const uint32_t idx1 = mesh.indices[index + 1];
				const uint32_t idx2 = mesh.indices[index + 2];

				drawEdge(mesh.vertices[idx0].position, mesh.vertices[idx1].position);
				drawEdge(mesh.vertices[idx1].position, mesh.vertices[idx2].position);
				drawEdge(mesh.vertices[idx2].position, mesh.vertices[idx0].position);
			}
		}
	}

	glEnd();
}

void PreviewWindow::draw_center_axis()
{
	GLsizei viewW, viewH;
	this->GetSize(&viewW, &viewH);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	const viewport_camera& cam = get_impl().cam;

	const float axisSize = 60.0F;
	const float axisSizeHalf = axisSize / 2;
	const float margin = 15.0F;
	const float centerX = margin + axisSizeHalf;
	const float centerY = margin + axisSizeHalf;

	const glm::vec3 right = cam.right() * axisSizeHalf;
	const glm::vec3 forward = cam.forward() * -axisSizeHalf;
	const glm::vec3 up = cam.up() * axisSizeHalf;

	glLoadIdentity();
	glOrtho(0, viewW, 0, viewH, -1, 1);

	glLineWidth(2.0F);
	glBegin(GL_LINES);

	// X (red)
	glColor3f(1.0F, 0.0F, 0.0F);
	glVertex2f(centerX, centerY);
	glVertex2f(centerX - right.x, centerY + right.y);

	// Y (green)
	glColor3f(0.0F, 1.0F, 0.0F);
	glVertex2f(centerX, centerY);
	glVertex2f(centerX + up.x, centerY - up.y);

	// Z (blue)
	glColor3f(0.0F, 0.0F, 1.0F);
	glVertex2f(centerX, centerY);
	glVertex2f(centerX + forward.x, centerY - forward.y);

	glEnd();
}

void PreviewWindow::SetModel(const cxmf::IModel* model)
{
	get_impl().model = model;
	this->Refresh();
}
