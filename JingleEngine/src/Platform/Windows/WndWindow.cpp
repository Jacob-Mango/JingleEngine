#include "Platform/Windows/WndWindow.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

#include "Core/Application.h"
#include "Core/ModuleManager.h"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

static const WORD MAX_CONSOLE_LINES = 500;

BEGIN_MODULE_LINK(WndWindow);
END_MODULE_LINK();

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == 33361)
	{
		return;
	}
	
	std::cout << "ERROR" << std::endl;
	std::cout << "Source: " << source << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Severity: " << severity << std::endl;
	std::cout << "Message: " << std::string(message, length) << std::endl;
}

HINSTANCE WndWindow::s_Instance;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WndWindow::Create(const WindowDesc& desc)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = s_Instance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"oglversionchecksample";
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
	{
		return -1;
	}

	m_Window = CreateWindowW(wc.lpszClassName, L"openglversioncheck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0, s_Instance, 0);
	m_DeviceContext = GetDC(m_Window);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormal = ChoosePixelFormat(m_DeviceContext, &pfd);
	if (pixelFormal == 0)
	{
		return -2;
    }

	if (!SetPixelFormat(m_DeviceContext, pixelFormal, &pfd))
	{
		return -3;
	}

	HGLRC tempContext = wglCreateContext(m_DeviceContext);
	wglMakeCurrent(m_DeviceContext, tempContext);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return -4;
	}

	if (wglewIsSupported("WGL_ARB_create_context") != 1)
	{
		return -5;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	m_GLContext = wglCreateContextAttribsARB(m_DeviceContext, 0, attribs);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);

    wglMakeCurrent(m_DeviceContext, m_GLContext);

	int glVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

	std::cout << "OpenGL Version: " << glVersion[0] << "." << glVersion[1] << std::endl;
	//JS_INFO("OpenGL Version %d.%d", glVersion[0], glVersion[1]);

/*
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

	if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
	{
		return -4;
	}

	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		return -5;
	}
*/

	GLint flags;
	GL(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		GL(glEnable(GL_DEBUG_OUTPUT));
		GL(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GL(glDebugMessageCallback(glDebugOutput, nullptr)); // segfault on this line
		GL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
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
    ImGui_ImplWin32_Init(m_Window);
	//ImGui_ImplSDL2_InitForOpenGL(m_SDLWindow, (SDL_GLContext)m_GLContext);
	ImGui_ImplOpenGL3_Init("#version 130");

	SetVsync(false);

	return 0;
}

WndWindow::~WndWindow()
{
	wglMakeCurrent(m_DeviceContext, NULL);
	wglDeleteContext(m_GLContext);
	ReleaseDC(m_Window, m_DeviceContext);
	DestroyWindow(m_Window);
	/* stop event queue thread */
	PostQuitMessage(0);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

    DestroyWindow(m_Window);
    UnregisterClass(L"openglversioncheck", s_Instance);

	//SDL_GL_DeleteContext((SDL_GLContext)m_GLContext);
	//SDL_DestroyRenderer(m_SDLRenderer);
	//SDL_DestroyWindow(m_SDLWindow);
	//SDL_Quit();
}

bool WndWindow::IsVsync() const
{
	return false;
	//return SDL_GL_GetSwapInterval() != 0;
}

void WndWindow::SetVsync(bool enabled)
{
	//SetSwapInterval(enabled);
}

void WndWindow::SetSize(std::pair<int, int> size)
{
	//SDL_SetWindowSize(m_SDLWindow, size.first, size.second);
}

std::pair<int, int> WndWindow::GetSize()
{
	int width, height;
	//SDL_GetWindowSize(m_SDLWindow, &width, &height);
	return { width, height };
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		break;
	case WM_CLOSE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

	/*
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
	*/
}

void WndWindow::Begin()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void WndWindow::End()
{
	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    wglMakeCurrent(m_DeviceContext, m_GLContext);
    SwapBuffers(m_DeviceContext);
}

#ifdef _DEBUG
void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE* fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}
#endif
