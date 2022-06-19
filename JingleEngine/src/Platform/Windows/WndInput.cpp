#include "Platform/Windows/WndWindow.h"

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Window.h"

#include "Core/ModuleManager.h"

#include "Input/Input.h"

bool g_CursorVisible = true;
HCURSOR g_ArrowCursor = NULL;

bool g_CursorWantsSet;
std::pair<int, int> g_CursorPosition;

bool g_CursorInViewport;
std::pair<int, int> g_ViewportMiddle;
std::pair<int, int> g_LastCursorPosition;

void Input::SetMousePosition(int x, int y)
{
	g_CursorWantsSet = true;
	g_CursorPosition = { x, y };
}

std::pair<int, int> Input::GetMousePosition()
{
	return g_CursorPosition;
}

std::pair<int, int> Input::GetMouseDelta()
{
	return { WndWindow::s_LastMouseDeltaX, WndWindow::s_LastMouseDeltaY };
}

bool Input::IsCursorInViewport()
{
	return g_CursorInViewport;
}

void Input::SetViewportMiddle(std::pair<int, int> middle)
{
	g_CursorInViewport = true;
	g_ViewportMiddle = middle;
}

void Input::ClearViewport()
{
	g_CursorInViewport = false;
}

void Input::ShowCursor(bool show)
{
	if (!g_CursorVisible && show)
	{
		g_CursorWantsSet = true;
		g_CursorPosition = g_LastCursorPosition;
	}
	else if (g_CursorVisible && !show)
	{
		g_LastCursorPosition = g_CursorPosition;
	}

	g_CursorVisible = show;
}

bool Input::IsCursorVisible()
{	
	return g_CursorVisible;
}

void Input::Update()
{
	WndWindow* window = ModuleManager::Get<WndWindow>();

	if (WndWindow::s_MouseDeltaX != 0 || WndWindow::s_MouseDeltaY != 0)
	{
		MouseMoveEventArgs args;
		args.X = WndWindow::s_MouseDeltaX;
		args.Y = WndWindow::s_MouseDeltaY;

		Application::Get()->OnMouseMove.Invoke(window, args);
		Application::Get()->OnEvent(window, args);
	}

	WndWindow::s_LastMouseDeltaX = WndWindow::s_MouseDeltaX;
	WndWindow::s_LastMouseDeltaY = WndWindow::s_MouseDeltaY;

	WndWindow::s_MouseDeltaX = 0;
	WndWindow::s_MouseDeltaY = 0;

	if (!g_ArrowCursor)
	{
		g_ArrowCursor = LoadCursor(NULL, IDC_ARROW);
	}

	if (g_CursorVisible)
	{
		SetCursor(g_ArrowCursor);
	}
	else
	{
		SetCursor(NULL);

		if (g_CursorInViewport)
		{
			SetMousePosition(g_ViewportMiddle.first, g_ViewportMiddle.second);
		}
	}

	if (g_CursorWantsSet)
	{
		g_CursorWantsSet = false;

		POINT mousePoint = { g_CursorPosition.first, g_CursorPosition.second };

		::SetCursorPos(mousePoint.x, mousePoint.y);
	}
	else
	{
		POINT mousePoint;
		bool has_mouse_screen_pos = ::GetCursorPos(&mousePoint) != 0;

		g_CursorPosition = { mousePoint.x, mousePoint.y };
	}
}
