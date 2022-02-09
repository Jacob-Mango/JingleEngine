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

#include <imgui.h>

class Application : public BaseClass
{
private:
	bool m_IsRunning = false;
	bool m_RequestingExit = false;
	
	bool m_Debug;

	uint64_t m_FPS = 0;
	double m_DeltaTime = 0;

	Ref<Scene> m_Scene;

	Window* m_Window;

	std::map<std::string, Ref<EntityType>> m_BaseEntityTypes;
	std::map<std::string, Ref<EntityType>> m_EntityTypes;

	//todo: move into a material module
	std::map<std::string, Ref<Material>> m_Materials;

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
	~Application();

	void RegisterBaseEntityType(std::string name, EntityType* type);
	void RegisterEntityType(Config& config);

	Ref<EntityType> GetEntityType(std::string type);

	template<typename T>
	Ref<T> GetEntityType()
	{
		return GetEntityType(T::BaseName()).As<T>();
	}

	//todo: move into a material module
	void AddMaterial(Config& config);
	Ref<Material> GetMaterial(std::string material);

	//todo: make framebuffers a type of entity?
	Ref<Framebuffer> CreateFramebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width = 512, unsigned int height = 512, bool cubeMap = false);

	void Start();
	void Stop();

	void RequestExit();
	void ClearExitRequest();
	bool RequestingExit();

	bool IsDebug() const;
	void SetDebug(bool enabled);

	uint64_t GetFPS() const;

	void SetScene(Ref<Scene> Scene);
	Ref<Scene> GetScene() const;

	Window* GetWindow();

	virtual int Init();
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnTick(double DeltaTime);
};

extern Application* g_Application;
