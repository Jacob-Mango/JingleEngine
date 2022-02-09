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

Application::Application()
{
	g_Application = this;
}

Application::~Application()
{
	BindingManager::Destroy();
	AssetManager::Destroy();

	g_Application = nullptr;
}

int Application::Init()
{
	WindowDesc desc;
	desc.FullScreen = false;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Title = "Test Window";
	m_Window = Window::Create(desc);

	OnWindowClose += [this](BaseClass* sender, WindowCloseEventArgs args) {
		RequestExit();
	};

	OnWindowResize += [this](BaseClass* sender, WindowResizeEventArgs args) {
	//	SetSize(args.Width, args.Height);
	};

	BindingManager::Init();
	AssetManager::Init();

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

#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Time;

Time CurrentTime()
{
	return std::chrono::high_resolution_clock::now();
}

double ElapsedTime(Time* time)
{
	Time current = CurrentTime();
	double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(current - *time).count() * 0.001 * 0.001 * 0.001;
	*time = current;
	return delta;
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

	Time time = CurrentTime();

	double frameTime = 0;
	uint64_t frames = 0;

	while (m_IsRunning)
	{
		m_DeltaTime = ElapsedTime(&time);

		frameTime += m_DeltaTime;
		frames++;
		if (frameTime >= 1)
		{
			m_FPS = frames;
			frameTime = 0;
			frames = 0;
		}

		BindingManager::Update();

		m_Window->PollEvents();

		m_Window->Begin();

		OnTick((float)m_DeltaTime);

		m_Window->End();

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

bool Application::IsDebug() const
{
	return m_Debug;
}

void Application::SetDebug(bool enabled)
{
	m_Debug = enabled;
}

Window* Application::GetWindow()
{
	return m_Window;
}

uint64_t Application::GetFPS() const
{
	return m_FPS;
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
