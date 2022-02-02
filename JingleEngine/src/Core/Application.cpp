#include "Application.h"

#include "Core.h"

#define GLT_IMPLEMENTATION
#include <gltext.h>

#include "../Asset/AssetManager.h"

#include "Binding.h"
#include "Config.h"
#include "Entity.h"
#include "Scene.h"

#include "../Rendering/Framebuffer.h"
#include "../Rendering/Material.h"
#include "../Rendering/Renderer.h"

Application* g_Application;

Application::Application(std::string title, bool fullscreen, int width, int height) : m_Title(title), m_Fullscreen(fullscreen), m_WindowWidth(width), m_WindowHeight(height)
{
	g_Application = this;
}

Application::~Application()
{
	AssetManager::Destroy();

	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyRenderer(m_SDLRenderer);
	SDL_DestroyWindow(m_SDLWindow);
	SDL_Quit();

	g_Application = nullptr;
}

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == 33361)
		return;

	std::cout << "ERROR" << std::endl;
	std::cout << "Source: " << source << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Severity: " << severity << std::endl;
	std::cout << "Message: " << std::string(message, length) << std::endl;
}

int Application::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_CaptureMouse(SDL_TRUE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int sdlFlags = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &sdlFlags);
	sdlFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, sdlFlags);

	int windowsFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	int posFlag = SDL_WINDOWPOS_CENTERED;

	if (m_Fullscreen)
	{
		windowsFlags |= SDL_WINDOW_FULLSCREEN;
		posFlag = SDL_WINDOWPOS_UNDEFINED;

		m_WindowWidth = 0;
		m_WindowHeight = 0;
	}

	m_SDLWindow = SDL_CreateWindow(m_Title.c_str(), posFlag, posFlag, m_WindowWidth, m_WindowHeight, windowsFlags);
	if (m_SDLWindow == nullptr)
	{
		std::cerr << "ERROR: Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_SDLRenderer == nullptr)
	{
		std::cerr << "ERROR: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cerr << "ERROR: Failed to initialize the OpenGL context." << std::endl;
		return -1;
	}

	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		std::cerr << "ERROR: SDL coudn't retrieve current display mode details. SDL Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	m_ScreenWidth = dm.w;
	m_ScreenHeight = dm.h;

	if (m_Fullscreen)
	{
		m_WindowWidth = m_ScreenWidth;
		m_WindowHeight = m_ScreenHeight;
	}

	std::cout << "MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;

	BindingManager::Init(this);
	AssetManager::Init();

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr); // segfault on this line
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	if (!gltInit())
	{
		std::cerr << "Warning: Failed to initialize glText" << std::endl;
		return -1;
	}

	m_Renderer = new Renderer();

	return 0;
}
void Application::RegisterBaseEntityType(std::string name, EntityType* type)
{
	m_BaseEntityTypes.insert({ name, type });
}

void Application::RegisterEntityType(Config& config)
{
	EntityType* type = nullptr;
	Config* base = &config;
	while (base != nullptr && type == nullptr)
	{
		auto it = m_BaseEntityTypes.find(base->m_Name);
		if (it == m_BaseEntityTypes.end())
		{
			base = base->GetBase();
			continue;
		}

		type = it->second;
	}

	if (type == nullptr)
		return;

	type = type->CreateType();
	type->Load(config);

	m_EntityTypes[config.m_Name] = type;
}

Ref<EntityType> Application::GetEntityType(std::string type)
{
	auto it = m_EntityTypes.find(type);
	if (it == m_EntityTypes.end())
		return nullptr;

	return it->second;
}

Ref<Framebuffer> Application::CreateFramebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width, unsigned int height, bool cubeMap)
{
	OUT_LINE("Application::CreateFramebuffer");

	Ref<Framebuffer> buffer = new Framebuffer(name, attachments, width, height, cubeMap);
	m_Framebuffers.push_back(buffer);
	return buffer;
}

void Application::Start()
{
	if (m_IsRunning)
	{
		std::cout << "Failed to start Application, already running." << std::endl;
		return;
	}

	if (Init() != 0)
	{
		std::cout << "Failed to start Application, initialize failed." << std::endl;
		return;
	}

	m_IsRunning = true;

	OnStart();

	if (m_Scene != nullptr)
	{
		m_Scene->OnStart();
	}

	m_CurrentTime = SDL_GetPerformanceCounter();

	double frameTime = 0;
	Uint64 frames = 0;

	while (m_IsRunning)
	{
		Uint64 previousTime = m_CurrentTime;
		m_CurrentTime = SDL_GetPerformanceCounter();
		m_DeltaTime = (double)(m_CurrentTime - previousTime) / (double)SDL_GetPerformanceFrequency();

		frameTime += m_DeltaTime;
		frames++;
		if (frameTime >= 1)
		{
			m_FPS = frames;
			frameTime = 0;
			frames = 0;
		}

		BindingManager::Update();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			HandleEvents(event);
		}

		OnTick((float)m_DeltaTime);

		SDL_GL_SwapWindow(m_SDLWindow);
	}

	if (m_IsRunning)
	{
		Stop();
	}
}

void Application::Stop()
{
	if (!m_IsRunning)
	{
		std::cout << "Failed to stop Application, already stopped." << std::endl;
		return;
	}

	m_IsRunning = false;
	m_RequestingExit = false;

	OnStop();

	if (m_Scene != nullptr)
	{
		m_Scene->OnStop();
	}
}

void Application::RequestExit()
{
	m_RequestingExit = true;
}

void Application::ClearExitRequest()
{
	m_RequestingExit = false;
}

bool Application::RequestingExit()
{
	return m_RequestingExit;
}

bool Application::IsVsync() const
{
	return m_Vsync;
}

void Application::SetVsync(bool enabled)
{
	m_Vsync = enabled;

	SDL_GL_SetSwapInterval(m_Vsync);
}

bool Application::IsDebug() const
{
	return m_Debug;
}

void Application::SetDebug(bool enabled)
{
	m_Debug = enabled;
}

void Application::DrawText(std::string text)
{
	GLTtext* glttext = gltCreateText();
	gltSetText(glttext, text.c_str());
	m_Texts.push_back(glttext);
}

void Application::UpdateDebug(double DeltaTime)
{
	gltBeginDraw();
	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < m_Texts.size(); i++)
	{
		gltDrawText2D(m_Texts[i], 5.f, 5.f + (i * 20.f), 1.f);
		gltDestroyText(m_Texts[i]);
	}

	gltEndDraw();

	m_Texts.clear();
}

void Application::SetSize(std::pair<int, int> size)
{
	m_WindowWidth = size.first;
	m_WindowHeight = size.second;
}

std::pair<int, int> Application::GetSize()
{
	return { m_WindowWidth, m_WindowHeight };
}

SDL_Window* Application::GetWindow()
{
	return m_SDLWindow;
}

Uint64 Application::GetFPS() const
{
	return m_FPS;
}

Uint64 Application::GetCurrentTime() const
{
	return m_CurrentTime;
}

void Application::SetScene(Ref<Scene> Scene)
{
	if (m_IsRunning && m_Scene != nullptr)
	{
		m_Scene->OnStop();
	}

	m_Scene = Scene;

	if (m_IsRunning && m_Scene != nullptr)
	{
		m_Scene->OnStart();
	}
}

Ref<Scene> Application::GetScene() const
{
	return m_Scene;
}

void Application::HandleEvents(SDL_Event& event)
{
	BindingManager::OnEvent(event);

	switch (event.type)
	{
	case SDL_QUIT:
		RequestExit();
		break;
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SetSize({event.window.data1, event.window.data2});
		}
		break;
	}
}

void Application::OnStart()
{
}

void Application::OnStop()
{
}

void Application::OnTick(double DeltaTime)
{
	if (RequestingExit())
		Stop();
}
