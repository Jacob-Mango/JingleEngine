#include "Application.h"

#include "Asset/AssetModule.h"

#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/Event.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "Rendering/Framebuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Renderer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;
}

Application::~Application()
{
}

Application* Application::Get()
{
	return s_Instance;
}

int Application::Initialize()
{
	WindowDesc desc;
	desc.FullScreen = false;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Title = "Test Window";
	m_Window = Window::Create(desc);

	OnWindowClose += [this](BaseClass* sender, WindowCloseEventArgs args) {
		Shutdown();
	};

	OnWindowResize += [this](BaseClass* sender, WindowResizeEventArgs args) {
		std::cout << "Resized width=" << args.Width << " height=" << args.Height << std::endl;
	};

	return 0;
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

void Application::Run()
{
	m_IsRunning = true;

	Time time = CurrentTime();

	double frameTime = 0;
	uint64_t frames = 0;

	for (auto [name, module] : m_Modules)
	{
		module->OnCreate();
	}

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

		m_Window->PollEvents();

		m_Window->Begin();

		OnTick((float)m_DeltaTime);

		m_Window->End();

		if (m_Shutdown)
		{
			m_IsRunning = false;
		}
	}

	for (auto [name, module] : m_Modules)
	{
		module->OnDestroy();
	}
}

void Application::Shutdown()
{
	m_Shutdown = true;
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

void Application::OnEvent(BaseClass* sender, const EventArgs& args)
{
	for (auto [name, module] : m_Modules)
	{
		module->OnEvent(sender, args);
	}
}

void Application::OnTick(double DeltaTime)
{
	for (auto [name, module] : m_Modules)
	{
		module->OnTick(DeltaTime);
	}
}
