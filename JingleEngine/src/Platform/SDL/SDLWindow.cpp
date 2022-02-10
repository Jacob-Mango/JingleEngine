#define SDLWINDOW_NO_REGISTER
#include "Platform/SDL/SDLWindow.h"

#include "Core/Application.h"
#include "Core/ModuleManager.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
				   GLsizei length, const GLchar *message, const void *userParam)
{
	if (type == 33361)
		return;

	std::cout << "ERROR" << std::endl;
	std::cout << "Source: " << source << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Severity: " << severity << std::endl;
	std::cout << "Message: " << std::string(message, length) << std::endl;
}

int SDLWindow::Create(const WindowDesc &desc)
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

	int width = desc.Width;
	int height = desc.Height;

	int windowsFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	int posFlag = SDL_WINDOWPOS_CENTERED;

	if (desc.FullScreen)
	{
		windowsFlags |= SDL_WINDOW_FULLSCREEN;
		posFlag = SDL_WINDOWPOS_UNDEFINED;

		width = 0;
		height = 0;
	}

	m_SDLWindow = SDL_CreateWindow(desc.Title.c_str(), posFlag, posFlag, width, height, windowsFlags);
	if (m_SDLWindow == nullptr)
	{
		return -2;
	}

	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_SDLRenderer == nullptr)
	{
		return -3;
	}

	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		return -4;
	}

	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		return -5;
	}

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr); // segfault on this line
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_SDLWindow, (SDL_GLContext)m_GLContext);
	ImGui_ImplOpenGL3_Init("#version 130");

	return 0;
}

SDLWindow::~SDLWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext((SDL_GLContext)m_GLContext);
	SDL_DestroyRenderer(m_SDLRenderer);
	SDL_DestroyWindow(m_SDLWindow);
	SDL_Quit();
}

bool SDLWindow::IsVsync() const
{
	return SDL_GL_GetSwapInterval() != 0;
}

void SDLWindow::SetVsync(bool enabled)
{
	SDL_GL_SetSwapInterval(enabled);
}

void SDLWindow::SetSize(std::pair<int, int> size)
{
	SDL_SetWindowSize(m_SDLWindow, size.first, size.second);
}

std::pair<int, int> SDLWindow::GetSize()
{
	int width, height;
	SDL_GetWindowSize(m_SDLWindow, &width, &height);
	return { width, height };
}

void SDLWindow::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		ImGuiIO& io = ImGui::GetIO();

		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			if (io.WantCaptureKeyboard) break;

			KeyPressEventArgs args;
			args.Key = event.key.keysym.sym;

			Application::Get()->OnKeyPress.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_KEYUP:
		{
			if (io.WantCaptureKeyboard) break;

			KeyReleaseEventArgs args;
			args.Key = event.key.keysym.sym;

			Application::Get()->OnKeyRelease.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_MOUSEMOTION:
		{
			if (io.WantCaptureMouse) break;

			MouseMoveEventArgs args;
			args.X = event.motion.x;
			args.Y = event.motion.y;

			Application::Get()->OnMouseMove.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (io.WantCaptureMouse) break;

			MouseButtonPressEventArgs args;
			args.Button = event.button.button;

			Application::Get()->OnMouseButtonPress.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (io.WantCaptureMouse) break;

			MouseButtonReleaseEventArgs args;
			args.Button = event.button.button;

			Application::Get()->OnMouseButtonRelease.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_MOUSEWHEEL:
		{
			if (io.WantCaptureMouse) break;

			MouseScrollEventArgs args;
			args.Direction = event.wheel.direction;
			args.X = event.wheel.x;
			args.Y = event.wheel.y;

			Application::Get()->OnMouseScroll.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_QUIT:
		{
			WindowCloseEventArgs args;

			Application::Get()->OnWindowClose.Invoke(this, args);
			Application::Get()->OnEvent(this, args);

			break;
		}
		case SDL_WINDOWEVENT:
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			{
				WindowResizeEventArgs args;

				args.Width = event.window.data1;
				args.Height = event.window.data2;
				
				Application::Get()->OnWindowResize.Invoke(this, args);
				Application::Get()->OnEvent(this, args);

				break;
			}
			}
			
			break;
		}
		}
	}
}

void SDLWindow::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void SDLWindow::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_SDLWindow);
}
