#include "Core.h"

#include "Planet/Planet.h"
#include "Planet/PlanetQuad.h"

class SpaceGame : public Application
{
private:
	bool m_WasMouseLocked = false;
	int m_FaceMode = GL_FILL;
	bool m_DepthTesting = true;
	bool m_BackFaceCulling = true;
	bool m_OSDMode = false;

	int m_BufferIndex = 0;

	Ref<Framebuffer> m_ScreenBuffer;
	Ref<Shader> m_ScreenShader;
	Ref<Mesh> m_ScreenMesh;

	Ref<Framebuffer> m_SceneBuffer;

public:
	int Init();

	virtual void OnStart() override;
	virtual void OnTick(double DeltaTime) override;
};

int SpaceGame::Init()
{
	if (int err = Application::Init() != 0)
	{
		return err;
	}

	EntityType::Register(this);
	CameraType::Register(this);
	LightType::Register(this);
	DebugType::Register(this);
	PlanetType::Register(this);
	PlanetQuadType::Register(this);

	BindingManager::RegisterCombos("exit", { {{KeyCode::ESCAPE, InputType::KEY}} });
	BindingManager::RegisterCombos("focus", { {{MouseCode::BUTTON_1, InputType::MOUSE}} });

	BindingManager::RegisterCombos("forward", { {{'w', InputType::KEY}} });
	BindingManager::RegisterCombos("backward", { {{'s', InputType::KEY}} });
	BindingManager::RegisterCombos("left", { {{'a', InputType::KEY}} });
	BindingManager::RegisterCombos("right", { {{'d', InputType::KEY}} });

	BindingManager::RegisterCombos("incline_increase", { {{KeyCode::UP, InputType::KEY}} });
	BindingManager::RegisterCombos("incline_decrease", { {{KeyCode::DOWN, InputType::KEY}} });
	BindingManager::RegisterCombos("roll_left", { {{KeyCode::LEFT, InputType::KEY}} });
	BindingManager::RegisterCombos("roll_right", { {{KeyCode::RIGHT, InputType::KEY}} });

	BindingManager::RegisterCombos("buffer_prev", { {{'o', InputType::KEY}} });
	BindingManager::RegisterCombos("buffer_next", { {{'p', InputType::KEY}} });

	BindingManager::RegisterCombos("toggle_facemode", { {{'f', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_backfaceculling", { {{'c', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_depthtesting", { {{'z', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_osd", { {{'h', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_vsync", { {{'v', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_debug", { {{'g', InputType::KEY}} });

	BindingManager::RegisterCombos("turbo", { {{KeyCode::LSHIFT, InputType::KEY}} });
	BindingManager::RegisterCombos("mouse_scroll", { {{MouseCode::WHEEL_UP, InputType::MOUSE}, {MouseCode::WHEEL_DOWN, InputType::MOUSE}} });

	return 0;
}

void SpaceGame::OnStart()
{
	Application::OnStart();

	SetScene(new Scene(this));

	m_ScreenShader = AssetManager::Get<Shader>("assets/shaders/screen");
	m_ScreenBuffer = CreateFramebuffer("Main", { TextureFormat::RGBA32 });

	m_ScreenShader->Bind();
	m_ScreenShader->Set("u_Screen", 0);
	m_ScreenShader->Unbind();

	m_SceneBuffer = CreateFramebuffer("Scene", { TextureFormat::RGBA32,TextureFormat::RGBA32, TextureFormat::DEPTH });

	{
		std::vector<glm::vec3> positions = {
			{-1.0f,  1.0f, 0.0f},
			{-1.0f, -1.0f, 0.0f},
			{ 1.0f,  1.0f, 0.0f},
			{ 1.0f, -1.0f, 0.0f},
		};

		std::vector<glm::vec2> uvs = {
			{0.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
			{1.0f, 0.0f},
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 1, 3, 2
		};

		m_ScreenMesh = new Mesh(nullptr, positions, uvs, indices);
	}

	Ref<Config> config = Config::Load("assets/scenes/game.scene");

	auto& types = (*config)["types"];
	for (int i = 0; i < types.Count; i++)
	{
		RegisterEntityType(types[i]);
	}

	GetScene()->LoadScene((*config)["entities"]);

	std::cout << "Loaded config " << std::endl;
}

void SpaceGame::OnTick(double DeltaTime)
{
	Application::OnTick(DeltaTime);

	if (BindingManager::Get("exit") >= BindingState::PRESSED)
	{
		if (Input::IsCursorVisible())
		{
			m_WasMouseLocked = true;
		}

		Input::ShowCursor(true);

		if (!m_WasMouseLocked)
		{
			RequestExit();
			return;
		}
	}
	else
	{
		m_WasMouseLocked = false;
	}

	if (BindingManager::Get("focus") >= BindingState::PRESSED)
	{
		if (!Input::IsCursorVisible())
		{
			Input::ShowCursor(false);
		}
	}

	if (BindingManager::Get("toggle_facemode") == BindingState::PRESSED)
	{
		m_FaceMode = m_FaceMode == GL_FILL ? GL_LINE : GL_FILL;
	}

	if (BindingManager::Get("toggle_backfaceculling") == BindingState::PRESSED)
	{
		m_BackFaceCulling = !m_BackFaceCulling;
	}

	if (BindingManager::Get("toggle_depthtesting") == BindingState::PRESSED)
	{
		m_DepthTesting = !m_DepthTesting;
	}

	if (BindingManager::Get("toggle_osd") == BindingState::PRESSED)
	{
		m_OSDMode = !m_OSDMode;
	}

	if (BindingManager::Get("toggle_vsync") == BindingState::PRESSED)
	{
	//	SetVsync(!IsVsync());
	}

	if (BindingManager::Get("toggle_debug") == BindingState::PRESSED)
	{
		SetDebug(!IsDebug());
	}

	if (BindingManager::Get("buffer_prev") == BindingState::PRESSED)
	{
		m_BufferIndex--;
	}

	if (BindingManager::Get("buffer_next") == BindingState::PRESSED)
	{
		m_BufferIndex++;
	}

	if (m_BufferIndex < -1)
		m_BufferIndex = m_Framebuffers.size() - 1;
	else if (m_BufferIndex >= m_Framebuffers.size())
		m_BufferIndex = -1;

	GetScene()->OnSimulate(DeltaTime, m_Renderer);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glPolygonMode(GL_FRONT_AND_BACK, m_FaceMode);

	glClearColor(0.0, 0.0, 0.5, 1.0);
	glClearDepth(1.0f);

	auto [width, height] = GetWindow()->GetSize();

	GetScene()->SetProjectionMatrix(glm::perspective(glm::radians(90.0f), (GLfloat)width / (GLfloat)height, 0.001f, 1000.0f));

	if (m_BackFaceCulling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (m_DepthTesting)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	//m_SceneBuffer->Resize(width, height);
	//m_ScreenBuffer->Resize(width, height);

	//m_SceneBuffer->Bind();
	//m_SceneBuffer->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_SceneBuffer->Unbind();

	//m_ScreenBuffer->Bind();
	//m_ScreenBuffer->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Renderer->Render(GetScene());

	//m_ScreenBuffer->Unbind();

	/*
	if (m_BufferIndex >= 0)
	{
		auto texture = m_Framebuffers[m_BufferIndex]->GetTexture(0);
		if (!texture)
			texture = m_Framebuffers[m_BufferIndex]->GetTexture(-1);
		if (texture)
		{
			m_ScreenShader->Bind();
			texture->Bind(0);
			m_ScreenMesh->Render();
			m_ScreenShader->Unbind();
		}
	}
	*/
}

int main(int argc, char** argv)
{
	OUT_LINE("main");
	SpaceGame* app = new SpaceGame();
	app->Start();
	return 0;
}
