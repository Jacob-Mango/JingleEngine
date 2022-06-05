#include "Platform/Windows/WndWindow.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

#include "Core/Application.h"
#include "Core/ModuleManager.h"

#include "Input/Input.h"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

static const WORD MAX_CONSOLE_LINES = 500;

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((unsigned short) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((unsigned short) 0x02)
#endif

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
long WndWindow::s_MouseDeltaX;
long WndWindow::s_MouseDeltaY;

WndWindow* g_tmp_Window;

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

	m_Width = desc.Width;
	m_Height = desc.Height;

	std::wstring title = std::wstring(desc.Title.begin(), desc.Title.end());

	m_Window = CreateWindowW(wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, m_Width, m_Height, 0, 0, s_Instance, 0);
	m_DeviceContext = GetDC(m_Window);

	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = m_Window;
	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));

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

	g_tmp_Window = this;

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
	return { m_Width, m_Height };
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WndWindow* window = g_tmp_Window;
	if (window == nullptr)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	if (Input::IsCursorVisible() && ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();

	switch (message)
	{
	case WM_SIZE:
	{
		window->m_Width = LOWORD(lParam);
		window->m_Height = HIWORD(lParam);

		WindowResizeEventArgs args;
		args.Width = window->m_Width;
		args.Height = window->m_Height;

		Application::Get()->OnWindowResize.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_KEYDOWN:
	{
		if (io.WantCaptureKeyboard) break;

		KeyPressEventArgs args;
		args.Key = wParam;

		Application::Get()->OnKeyPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
		break;
	}
	case WM_KEYUP:
	{
		if (io.WantCaptureKeyboard) break;

		KeyReleaseEventArgs args;
		args.Key = wParam;

		Application::Get()->OnKeyRelease.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
		break;
	}
	case WM_INPUT:
	{
		if (io.WantCaptureMouse) break;

		unsigned size = sizeof(RAWINPUT);
		static RAWINPUT raw[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

		if (raw->header.dwType != RIM_TYPEMOUSE)
		{
			break;
		}

		WndWindow::s_MouseDeltaX = raw->data.mouse.lLastX;
		WndWindow::s_MouseDeltaY = raw->data.mouse.lLastY;

		if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			MouseScrollEventArgs args;
			args.Direction = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
			args.X = WndWindow::s_MouseDeltaX;
			args.Y = WndWindow::s_MouseDeltaY;

			Application::Get()->OnMouseScroll.Invoke(window, args);
			Application::Get()->OnEvent(window, args);
		}
		else
		{
			MouseMoveEventArgs args;
			args.X = WndWindow::s_MouseDeltaX;
			args.Y = WndWindow::s_MouseDeltaY;

			Application::Get()->OnMouseMove.Invoke(window, args);
			Application::Get()->OnEvent(window, args);
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonPressEventArgs args;
		args.Button = MouseCode::MC_BUTTON_1;

		Application::Get()->OnMouseButtonPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_LBUTTONUP:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonReleaseEventArgs args;
		args.Button = MouseCode::MC_BUTTON_1;

		Application::Get()->OnMouseButtonRelease.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonPressEventArgs args;
		args.Button = MouseCode::MC_BUTTON_2;

		Application::Get()->OnMouseButtonPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_RBUTTONUP:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonReleaseEventArgs args;
		args.Button = MouseCode::MC_BUTTON_2;

		Application::Get()->OnMouseButtonRelease.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonPressEventArgs args;
		args.Button = MouseCode::MC_BUTTON_3;

		Application::Get()->OnMouseButtonPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_MBUTTONUP:
	{
		if (io.WantCaptureMouse) break;

		MouseButtonReleaseEventArgs args;
		args.Button = MouseCode::MC_BUTTON_3;

		Application::Get()->OnMouseButtonRelease.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_QUIT:
	case WM_CLOSE:
	{
		WindowCloseEventArgs args;

		Application::Get()->OnWindowClose.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return true;
}

void WndWindow::Begin()
{
	MSG event;
	while (PeekMessage(&event, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&event);
		DispatchMessage(&event);
	}

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

// Source: https://stackoverflow.com/questions/191842/how-do-i-get-console-output-in-c-with-a-windows-program
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