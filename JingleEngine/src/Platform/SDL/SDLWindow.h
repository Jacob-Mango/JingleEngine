#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Core/Window.h"

class SDLWindow : public Window
{
	friend class Input;
	friend class Window;

public:
	~SDLWindow();

	virtual bool IsVsync() const override;
	virtual void SetVsync(bool enabled) override;

	virtual void SetSize(std::pair<int, int> size) override;
	virtual std::pair<int, int> GetSize() override;

	virtual void PollEvents() override;
	
	virtual void Begin() override;
	virtual void End() override;

private:
	SDL_Window *m_SDLWindow = nullptr;
	SDL_Renderer *m_SDLRenderer = nullptr;
	SDL_GLContext m_GLContext = nullptr;
};
