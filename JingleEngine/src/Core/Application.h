#pragma once

class Config;
class Material;
class Framebuffer;

enum class TextureFormat;

#include "Core/Core.h"

#include <iostream>
#include <string>
#include <vector>

#include "Core/Event.h"
#include "Core/Module.h"

#include "Entities/Entity.h"

class Application : public JingleScript::Object
{
	DEFINE_CLASS(Application, JingleScript::Object);

private:
	static Application* s_Instance;

	bool m_IsRunning = false;
	bool m_Shutdown = false;
	
	bool m_Debug = false;

	uint64_t m_FPS = 0;
	double m_DeltaTime = 0;

	//! Temporary
	Entity* m_Scene = nullptr;

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

	Entity* GetScene() const { return m_Scene; }
	void SetScene(Entity* scene) { m_Scene = scene; }

public:
	int Initialize();

	void Run();
	void Shutdown();

	void OnEvent(BaseClass* sender, const EventArgs& args);
	void OnTick(double DeltaTime);

};
