#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Core/Core.h"
#include "Core/Window.h"

class WndWindow : public Window
{
	DEFINE_MODULE(WndWindow, Window);

	friend class Input;
	friend class Window;

public:
	WndWindow() {}
	virtual ~WndWindow();

	virtual int Create(const WindowDesc& desc) override;

	virtual bool IsVsync() const override;
	virtual void SetVsync(bool enabled) override;

	virtual void SetSize(std::pair<int, int> size) override;
	virtual std::pair<int, int> GetSize() override;

	virtual void Begin() override;
	virtual void End() override;

public:
	static long s_MouseDeltaX;
	static long s_MouseDeltaY;

private:
	static HINSTANCE s_Instance;

	HGLRC m_GLContext;
	HDC m_DeviceContext;
	HWND m_Window;

	int m_Width;
	int m_Height;

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

};
