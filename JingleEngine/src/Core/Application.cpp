#include "Application.h"

#include "Asset/AssetModule.h"

#include "Core/Core.h"
#include "Core/Binding.h"
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
	BindingManager::Destroy();
}

Application* Application::Get()
{
	return s_Instance;
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
		std::cout << "Resized width=" << args.Width << " height=" << args.Height << std::endl;
	};

	BindingManager::Init();

	return 0;
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

void Application::Run()
{
	m_IsRunning = true;

	Time time = CurrentTime();

	double frameTime = 0;
	uint64_t frames = 0;

	for (auto module : m_Modules)
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

		Input::Update();
		BindingManager::Update();

		m_Window->PollEvents();

		m_Window->Begin();

		OnTick((float)m_DeltaTime);

		m_Window->End();

		if (m_RequestingExit)
		{
			m_IsRunning = false;
			m_RequestingExit = false;
		}
	}

	for (auto module : m_Modules)
	{
		module->OnDestroy();
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

void Application::OnEvent(BaseClass* sender, const EventArgs& args)
{
	for (auto module : m_Modules)
	{
		module->OnEvent(sender, args);
	}
}

void Application::OnTick(double DeltaTime)
{
	static bool isSecondPress;

	if (BindingManager::Get("exit") == BindingState::PRESSED)
	{
		Input::ShowCursor(true);

		if (isSecondPress)
		{
			RequestExit();
			return;
		}

		isSecondPress = true;
	}
	else if (!Input::IsCursorVisible())
	{
		isSecondPress = false;
	}

	if (BindingManager::Get("focus") >= BindingState::PRESSED)
	{
		Input::ShowCursor(false);
	}

	for (auto module : m_Modules)
	{
		module->OnTick(DeltaTime);
	}
}
