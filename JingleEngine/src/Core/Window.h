#pragma once

#include "Core.h"

struct WindowDesc
{
	std::string Title = "";
	unsigned int Width;
	unsigned int Height;
	bool FullScreen;
};

class Window : public BaseClass
{
public:
	static Window *Create(const WindowDesc &desc = WindowDesc());

	virtual bool IsVsync() const = 0;
	virtual void SetVsync(bool enabled) = 0;
	virtual void SetSize(std::pair<int, int> size) = 0;
	virtual std::pair<int, int> GetSize() = 0;

	virtual void PollEvents() = 0;

	virtual void Begin() = 0;
	virtual void End() = 0;
};
