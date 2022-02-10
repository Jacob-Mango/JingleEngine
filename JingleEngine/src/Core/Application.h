#pragma once

class Scene;
class EntityType;
class Config;
class Material;
class Framebuffer;

enum class TextureFormat;

#include <iostream>
#include <string>
#include <vector>

#include "Core/Logging.h"
#include "Core/Reference.h"

#include "Core/Window.h"

#include "Core/Event.h"
#include "Core/Module.h"

#include <imgui.h>

class Application : public BaseClass
{
private:
	static Application* s_Instance;

	bool m_IsRunning = false;
	bool m_Shutdown = false;
	
	bool m_Debug = false;

	uint64_t m_FPS = 0;
	double m_DeltaTime = 0;

	Window* m_Window = nullptr;
	class Renderer* m_Renderer = nullptr;

	std::unordered_map<std::string, Module*> m_Modules;

public:
	EventHandler<KeyPressEventArgs> OnKeyPress;
	EventHandler<KeyReleaseEventArgs> OnKeyRelease;

	EventHandler<MouseButtonPressEventArgs> OnMouseButtonPress;
	EventHandler<MouseButtonReleaseEventArgs> OnMouseButtonRelease;
	EventHandler<MouseScrollEventArgs> OnMouseScroll;
	EventHandler<MouseMoveEventArgs> OnMouseMove;

	EventHandler<WindowResizeEventArgs> OnWindowResize;
	EventHandler<WindowCloseEventArgs> OnWindowClose;

public:
	Application();
	virtual ~Application();

	static Application* Get();

	bool IsDebug() const;
	void SetDebug(bool enabled);

	uint64_t GetFPS() const;
	
	Window* GetWindow();

public:
	int Initialize();

	void Run();
	void Shutdown();

	void OnEvent(BaseClass* sender, const EventArgs& args);
	void OnTick(double DeltaTime);

public:
	template<typename T>
	T* RegisterModule()
	{
		T* module = new T();
		m_Modules[T::StaticName()] = module;
		return module;
	}

	template<typename T>
	T* GetModule()
	{
		auto it = m_Modules.find(T::StaticName());
		if (it == m_Modules.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(it->second);
	}

};
