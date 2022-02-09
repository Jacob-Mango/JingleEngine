#pragma once

class Scene;
class EntityType;
class Config;
class Material;
class Framebuffer;

enum class TextureFormat;

struct GLTtext;

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <iostream>
#include <string>
#include <vector>

#include "Binding.h"
#include "Logging.h"
#include "Reference.h"

#include <imgui.h>

class Application
{
private:
	bool m_IsRunning = false;
	bool m_RequestingExit = false;

	std::string m_Title;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_WindowWidth;
	int m_WindowHeight;
	
	bool m_Fullscreen;
	bool m_Vsync;
	bool m_Debug;
	std::vector<GLTtext*> m_Texts;

	Uint64 m_CurrentTime = 0;
	Uint64 m_FPS = 0;
	double m_DeltaTime = 0;

	Ref<Scene> m_Scene;

	std::map<std::string, Ref<EntityType>> m_BaseEntityTypes;
	std::map<std::string, Ref<EntityType>> m_EntityTypes;

	std::map<std::string, Ref<Material>> m_Materials;

protected:
	SDL_Window* m_SDLWindow = nullptr;
	SDL_Renderer* m_SDLRenderer = nullptr;
	SDL_GLContext m_GLContext = nullptr;

	class Renderer* m_Renderer = nullptr;

	std::vector<Ref<Framebuffer>> m_Framebuffers;

public:
	Application(std::string title, bool fullscreen, int width, int height);
	~Application();

	void RegisterBaseEntityType(std::string name, EntityType* type);
	void RegisterEntityType(Config& config);

	Ref<EntityType> GetEntityType(std::string type);

	template<typename T>
	Ref<T> GetEntityType()
	{
		return GetEntityType(T::BaseName()).As<T>();
	}

	void AddMaterial(Config& config);
	Ref<Material> GetMaterial(std::string material);

	Ref<Framebuffer> CreateFramebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width = 512, unsigned int height = 512, bool cubeMap = false);

	void Start();
	void Stop();

	void RequestExit();
	void ClearExitRequest();
	bool RequestingExit();

	bool IsVsync() const;
	void SetVsync(bool enabled);

	bool IsDebug() const;
	void SetDebug(bool enabled);
	void DrawText(std::string text);

	void UpdateDebug(double DeltaTime);

	Uint64 GetFPS() const;
	Uint64 GetCurrentTime() const;

	void SetSize(std::pair<int, int> size);
	std::pair<int, int> GetSize();
	SDL_Window* GetWindow();

	void SetScene(Ref<Scene> Scene);
	Ref<Scene> GetScene() const;

	void HandleEvents(SDL_Event& event);

	virtual int Init();
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnTick(double DeltaTime);
};

extern Application* g_Application;
