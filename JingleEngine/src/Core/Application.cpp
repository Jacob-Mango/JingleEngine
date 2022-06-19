#include "Application.h"

#include "Asset/AssetModule.h"

#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/Event.h"
#include "Core/Window.h"
#include "Core/Module.h"
#include "Core/ModuleManager.h"

#include "Input/BindingModule.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "Rendering/Framebuffer.h"
#include "Rendering/ImGui.h"
#include "Rendering/Material.h"
#include "Rendering/Renderer.h"

#include <fstream>
#include <iostream>
#include <filesystem>

BEGIN_CLASS_LINK(Application)
	LINK_NAMED_VARIABLE(FPS, m_FPS);
	LINK_METHOD(GetScene);
END_CLASS_LINK()

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

STATIC_FUNCTION(Application, OnUpdate, void, double);

int Application::Initialize()
{
	// Once static is in JingleScript, this won't be needed
	LinkImGUI();
	LINK_NAMED_FUNCTION(GetApplication, Application::Get);

	ModuleManager::Initialize();

	OnWindowClose += [this](BaseClass* sender, WindowCloseEventArgs args) {
		Shutdown();
	};

	OnWindowResize += [this](BaseClass* sender, WindowResizeEventArgs args) {
		std::cout << "Resized width=" << args.Width << " height=" << args.Height << std::endl;
	};

	return 0;
}

void Application::Run()
{
	int error = 0;

	auto window = ModuleManager::Get<Window>();
	auto binding = ModuleManager::Get<BindingModule>();

	if (window)
	{
		WindowDesc desc;
		desc.FullScreen = false;
		desc.Width = 1280;
		desc.Height = 720;
		desc.Title = "JingleEngine";

		if ((error = window->Create(desc)) != 0)
		{
			return;
		}
	}

	ModuleManager::On([](Module* module)
		{
			std::cout << "Loaded Module ";

			std::cout << "[" << module->GetType()->Name() << "] ";

			std::cout << std::endl;
		});

	ModuleManager::On([](Module* module)
		{
			module->OnPreInitialize();
		});

	ModuleManager::On([](Module* module)
		{
			module->OnInitialize();
		});

	m_IsRunning = true;

	Time time;

	double frameTime = 0;
	uint64_t frames = 0;

	ImGuiRenderEventArgs imguiArgs;

	while (m_IsRunning)
	{
		m_DeltaTime = time.Elapsed();

		frameTime += m_DeltaTime;
		frames++;
		if (frameTime >= 1)
		{
			m_FPS = frames;
			frameTime = 0;
			frames = 0;

			std::cout << "FPS: " << m_FPS << std::endl;
		}

		if (window)
		{
			window->Begin();
		}

		if (binding)
		{
			binding->Process(m_DeltaTime);
		}

		OnTick((float)m_DeltaTime);

		imguiArgs.DeltaTime = m_DeltaTime;
		Application::Get()->OnEvent(this, imguiArgs);

		if (window)
		{
			window->End();
		}

		if (m_Shutdown)
		{
			m_IsRunning = false;
		}
	}

	ModuleManager::On([](Module* module)
		{
			module->OnDestroy();
		});
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

uint64_t Application::GetFPS() const
{
	return m_FPS;
}

void Application::OnEvent(BaseClass* sender, const EventArgs& args)
{
	ModuleManager::On([sender, &args](Module* module)
		{
			module->OnEvent(sender, args);
		});
}

void Application::OnTick(double DeltaTime)
{
	if (m_Scene)
	{
		m_Scene->OnTick(DeltaTime);
	}

	Script_OnUpdate[this](DeltaTime);

	ModuleManager::On([DeltaTime](Module* module)
		{
			module->OnTick(DeltaTime);
		});
}
