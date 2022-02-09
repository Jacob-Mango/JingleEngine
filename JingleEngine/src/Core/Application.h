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

#include "Binding.h"
#include "Logging.h"
#include "Reference.h"

#include "Window.h"

#include "Event.h"
#include "Module.h"

#include <imgui.h>

class Application : public BaseClass
{
private:
	static Application* s_Instance;

	bool m_IsRunning = false;
	bool m_RequestingExit = false;
	
	bool m_Debug;

	uint64_t m_FPS = 0;
	double m_DeltaTime = 0;

	Window* m_Window;

	//todo: move into a material module
	std::map<std::string, Ref<Material>> m_Materials;

	std::vector<Module*> m_Modules;

public:
	EventHandler<KeyPressEventArgs> OnKeyPress;
	EventHandler<KeyReleaseEventArgs> OnKeyRelease;

	EventHandler<MouseButtonPressEventArgs> OnMouseButtonPress;
	EventHandler<MouseButtonReleaseEventArgs> OnMouseButtonRelease;
	EventHandler<MouseScrollEventArgs> OnMouseScroll;
	EventHandler<MouseMoveEventArgs> OnMouseMove;

	EventHandler<WindowResizeEventArgs> OnWindowResize;
	EventHandler<WindowCloseEventArgs> OnWindowClose;

protected:
	class Renderer* m_Renderer = nullptr;

	std::vector<Ref<Framebuffer>> m_Framebuffers;

public:
	Application();
	virtual ~Application();

	static Application* Get();

	template<typename T>
	T* RegisterModule()
	{
		T* module = new T();
		m_Modules.push_back(module);
		return module;
	}

	template<typename T>
	T* GetModule()
	{

	}

	//todo: move into a material module
	void AddMaterial(Config* config);
	Ref<Material> GetMaterial(std::string material);

	//todo: make framebuffers a type of entity?
	Ref<Framebuffer> CreateFramebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width = 512, unsigned int height = 512, bool cubeMap = false);

	void Run();

	void RequestExit();
	void ClearExitRequest();
	bool RequestingExit();

	bool IsDebug() const;
	void SetDebug(bool enabled);

	uint64_t GetFPS() const;

	Window* GetWindow();

	int Init();

	void OnEvent(BaseClass* sender, const EventArgs& args);
	void OnTick(double DeltaTime);


};
