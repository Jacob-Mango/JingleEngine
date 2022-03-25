#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

class SDLWindow : public Window
{
	DEFINE_MODULE(SDLWindow, Window);

	friend class Input;
	friend class Window;

public:
	SDLWindow() {}
	virtual ~SDLWindow();

	virtual int Create(const WindowDesc& desc) override;

	virtual bool IsVsync() const override;
	virtual void SetVsync(bool enabled) override;

	virtual void SetSize(std::pair<int, int> size) override;
	virtual std::pair<int, int> GetSize() override;

	virtual void PollEvents() override;
	
	virtual void Begin() override;
	virtual void End() override;

private:
	struct SDL_Window* m_SDLWindow = nullptr;
	struct SDL_Renderer* m_SDLRenderer = nullptr;
	void* m_GLContext = nullptr;
};
