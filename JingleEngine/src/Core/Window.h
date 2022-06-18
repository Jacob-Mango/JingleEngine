#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

struct WindowDesc
{
	std::string Title = "";
	unsigned int Width;
	unsigned int Height;
	bool FullScreen;
};

class Window : public Module
{
	DEFINE_VIRTUAL_MODULE(Window, Module);

public:
	Window() {}

	virtual int Create(const WindowDesc &desc = WindowDesc()) = 0;

	virtual void SetVsync(bool enabled) = 0;
	virtual bool IsVsync() const = 0;

	virtual void SetSize(std::pair<int, int> size) = 0;
	virtual std::pair<int, int> GetSize() = 0;

	virtual void SetFullscreen(bool fullscreen) = 0;
	virtual bool IsFullscreen() = 0;

	virtual void SetMaximized(bool maximized) = 0;
	virtual bool IsMaximized() = 0;

	virtual void Begin() = 0;
	virtual void End() = 0;
};
