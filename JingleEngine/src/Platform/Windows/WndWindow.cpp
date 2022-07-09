#include "Platform/Windows/WndWindow.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>

#include "Core/Application.h"
#include "Core/ModuleManager.h"

#include "Editor/EditorModule.h"

#include "Input/Input.h"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((unsigned short) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((unsigned short) 0x02)
#endif

BEGIN_MODULE_LINK(WndWindow);
	LINK_CONSTRUCTOR();
END_MODULE_LINK();

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == 33361)
	{
		return;
	}

	JS_ERROR("OpenGL Error: Source={} Type={} Severity={} Message={}", source, type, severity, std::string(message, length));
}

long WndWindow::s_MouseDeltaX;
long WndWindow::s_MouseDeltaY;

long WndWindow::s_LastMouseDeltaX;
long WndWindow::s_LastMouseDeltaY;

bool g_HasTitleBar = true;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WndWindow::Create(const WindowDesc& desc)
{
	JS_TRACE(Tracers::Platform);

	m_GLContext = NULL;
	m_Window = NULL;
	m_Instance = NULL;

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_Instance;
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

	DWORD dwStyle = g_HasTitleBar ? WS_OVERLAPPEDWINDOW : 0;
	DWORD dwExStyle = 0;

	m_Window = CreateWindowExW(dwExStyle, wc.lpszClassName, title.c_str(), dwStyle, 0, 0, m_Width, m_Height, 0, 0, m_Instance, 0);
	
	if (!g_HasTitleBar)
	{
    	SetWindowLong(m_Window, GWL_STYLE, dwStyle);
    	SetWindowLong(m_Window, GWL_EXSTYLE, dwExStyle);
	}

	ShowWindow(m_Window, SW_SHOW);
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

	JS_TINFO("OpenGL Version {}.{}", glVersion[0], glVersion[1]);

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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

	style.WindowMenuButtonPosition = ImGuiDir_None; // ImGuiDir_Right; // Causes crash

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_Window, m_GLContext);
	ImGui_ImplOpenGL3_Init("#version 120");

	//SetVsync(false);

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
	ImGui::DestroyContext();

	DestroyWindow(m_Window);
	UnregisterClass(L"openglversioncheck", m_Instance);
}

void WndWindow::SetVsync(bool enabled)
{
	wglSwapIntervalEXT(enabled);
}

bool WndWindow::IsVsync() const
{
	return wglGetSwapIntervalEXT() != 0;
}

void WndWindow::SetSize(std::pair<int, int> size)
{
	WINDOWPLACEMENT wi;
	GetWindowPlacement(m_Window, &wi);

	wi.ptMaxPosition.x = wi.ptMinPosition.x + size.first;
	wi.ptMaxPosition.y = wi.ptMinPosition.y + size.first;

	SetWindowPlacement(m_Window, &wi);
}

std::pair<int, int> WndWindow::GetSize()
{
	return { m_Width, m_Height };
}

void WndWindow::SetFullscreen(bool fullscreen)
{
	static bool forMetro = false;

	SetWindowPos(m_Window, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);

	// Save current window state if not already fullscreen.
	if (!m_Fullscreen.Active)
	{
		// Save current window information.  We force the window into restored mode
		// before going fullscreen because Windows doesn't seem to hide the
		// taskbar if the window is in the maximized state.
		m_Fullscreen.Maximized = IsMaximized();
		if (m_Fullscreen.Maximized)
		{
			SendMessage(m_Window, WM_SYSCOMMAND, SC_RESTORE, 0);
		}

		m_Fullscreen.Style = GetWindowLong(m_Window, GWL_STYLE);
		m_Fullscreen.ExStyle = GetWindowLong(m_Window, GWL_EXSTYLE);
		GetWindowRect(m_Window, &m_Fullscreen.Size);
	}

	m_Fullscreen.Active = fullscreen;

	if (m_Fullscreen.Active)
	{
		SetWindowLong(m_Window, GWL_STYLE, m_Fullscreen.Style & ~(WS_CAPTION | WS_THICKFRAME));
		SetWindowLong(m_Window, GWL_EXSTYLE, m_Fullscreen.ExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

		if (!forMetro)
		{
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(monitorInfo);
			GetMonitorInfo(MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST), &monitorInfo);

			long x = monitorInfo.rcMonitor.left;
			long y = monitorInfo.rcMonitor.top;
			long w = monitorInfo.rcMonitor.right;
			long h = monitorInfo.rcMonitor.bottom;

			SetWindowPos(m_Window, NULL, w, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(m_Window, GWL_STYLE, m_Fullscreen.Style);
		SetWindowLong(m_Window, GWL_EXSTYLE, m_Fullscreen.ExStyle);

		if (!forMetro)
		{
			long x = m_Fullscreen.Size.left;
			long y = m_Fullscreen.Size.top;
			long w = m_Fullscreen.Size.right;
			long h = m_Fullscreen.Size.bottom;

			SetWindowPos(m_Window, NULL, w, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}

		if (m_Fullscreen.Maximized)
		{
			SendMessage(m_Window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
}

bool WndWindow::IsFullscreen()
{
	return m_Fullscreen.Active;
}

void WndWindow::SetMaximized(bool maximized)
{
	
}

bool WndWindow::IsMaximized()
{
	return m_Maximized;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WndWindow* window = ModuleManager::Get<WndWindow>();
	if (window == nullptr || window->m_GLContext == NULL)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	ImGuiIO& io = ImGui::GetIO();

	switch (message)
	{
	case WM_ERASEBKGND:
	{
		//return 0; // Prevent flickering on window invalidation
	}
	case WM_SIZE:
	{
		window->m_Maximized = wParam == SIZE_MAXIMIZED;

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
    case WM_SYSKEYDOWN:
	{
		KeyPressEventArgs args;
		args.Key = (InputCode)wParam;

		Application::Get()->OnKeyPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
		break;
	}
    case WM_KEYUP:
    case WM_SYSKEYUP:
	{
		KeyReleaseEventArgs args;
		args.Key = (InputCode)wParam;

		Application::Get()->OnKeyRelease.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
		break;
	}
	case WM_INPUT:
	{
		unsigned size = sizeof(RAWINPUT);
		static RAWINPUT raw[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

		if (raw->header.dwType != RIM_TYPEMOUSE)
		{
			break;
		}

		WndWindow::s_MouseDeltaX += raw->data.mouse.lLastX;
		WndWindow::s_MouseDeltaY += raw->data.mouse.lLastY;

		if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			MouseScrollEventArgs args;
			args.Direction = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;

			Application::Get()->OnMouseScroll.Invoke(window, args);
			Application::Get()->OnEvent(window, args);
		}

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		MouseButtonPressEventArgs args;
		if (message == WM_LBUTTONDOWN) args.Button = InputCode::MC_BUTTON_1;
		else if (message == WM_RBUTTONDOWN) args.Button = InputCode::MC_BUTTON_2;
		else if (message == WM_MBUTTONDOWN) args.Button = InputCode::MC_BUTTON_3;

		Application::Get()->OnMouseButtonPress.Invoke(window, args);
		Application::Get()->OnEvent(window, args);

		break;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		MouseButtonReleaseEventArgs args;
		if (message == WM_LBUTTONUP) args.Button = InputCode::MC_BUTTON_1;
		else if (message == WM_RBUTTONUP) args.Button = InputCode::MC_BUTTON_2;
		else if (message == WM_MBUTTONUP) args.Button = InputCode::MC_BUTTON_3;

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
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
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
	ImGui_ImplWin32_NewFrame(Input::IsCursorInViewport());
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.ConfigWindowsResizeFromEdges = !g_HasTitleBar || io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	const char* windowName = "JingleEngine Editor";
	const ImGuiWindow* window = ImGui::FindWindowByName(windowName);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	if (window && !g_HasTitleBar)
	{
		ImVec2 size = window->Size;
		ImVec2 pos = window->Pos;

		SetWindowPos(m_Window, NULL, pos.x, pos.y, size.x, size.y, SWP_NOREDRAW | SWP_NOZORDER);
	}
	else
	{
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
	}

	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	if (g_HasTitleBar)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (ImGui::Begin(windowName, nullptr, windowFlags))
	{
		ImGui::PopStyleVar();
		
		if (g_HasTitleBar)
		{
			ImGui::PopStyleVar(2);
		}

		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

		auto editor = ModuleManager::Get<EditorModule>();
		if (editor)
		{
			if (ImGui::BeginMenuBar())
			{
				editor->RenderMenu();
			
				ImGui::EndMenuBar();
			}

			if (!editor->RenderEditors(1.0 / 144.0, dockspaceId))
			{
				WindowCloseEventArgs args;

				Application::Get()->OnWindowClose.Invoke(this, args);
				Application::Get()->OnEvent(this, args);
			}
		}
	}

	ImGui::End();
}

void WndWindow::End()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(m_DeviceContext, m_GLContext);
	SwapBuffers(m_DeviceContext);

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		auto backUpDC = m_DeviceContext;
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		m_DeviceContext = backUpDC;
		wglMakeCurrent(m_DeviceContext, m_GLContext);
	}
}
