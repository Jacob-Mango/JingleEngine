#include "Application.h"

#include "Asset/AssetModule.h"

#include "Core/Core.h"
#include "Core/Config.h"
#include "Core/Event.h"
#include "Core/Window.h"
#include "Core/Module.h"
#include "Core/ModuleManager.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "Rendering/Framebuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Renderer.h"

#include <JingleScript.h>

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;
}

Application::~Application()
{
	using namespace JingleScript;

	TypeManager::Destroy();
	GlobalManager::Destroy();
}

Application* Application::Get()
{
	return s_Instance;
}

int Application::Initialize()
{
	using namespace JingleScript;

	GlobalManager::Initialize();
	TypeManager::Initialize();

	bool success = true;

	std::string folder = "Assets/Scripts";
	std::string files[] = {
		"Core"
	};

	Ref<Parser> parser = new Parser();
	for (auto& file : files)
	{
		Ref<Lexer> lexer = Lexer::ParseFile(folder + "/" + file + ".jss");

		parser->SetLexer(lexer);
		success &= parser->Parse();
	}

	{
		Serializer serializer(folder + "/Parsed.jst");
		parser->GetGlobalNode()->Serialize(serializer);
	}

	if (success)
	{
		success &= Compiler::Compile(parser);
	}

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
	int error = 0;

	auto window = ModuleManager::Get<Window>();
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

			for (auto name : module->GetNames())
				std::cout << "[" << name << "] ";

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

		if (window)
		{
			window->PollEvents();
			window->Begin();
		}

		OnTick((float)m_DeltaTime);

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
	ModuleManager::On([DeltaTime](Module* module)
		{
			module->OnTick(DeltaTime);
		});
}
