#pragma once

#include "Core/Core.h"
#include "Core/Application.h"

#include "Scene/Entity.h"

class Camera;
class Texture;
class Light;

class Scene : public ConfigAsset
{
	SETUP_ASSET(Scene, ConfigAsset);
	
	friend class Entity;
	friend class Light;

	friend class Renderer;

	friend class SceneHierarchyPanel;
	
private:
	ConfigArray* m_EntitiesData;

	std::vector<Entity*> m_Entities;
	std::vector<Light*> m_Lights;

	unsigned int m_NextID = 0;

	Ref<Texture> m_Skybox;

	Camera* m_Camera = nullptr;

public:
	Scene();
	~Scene();

public:
	virtual bool OnLoad() override;

	void OnStart();
	void OnStop();
	void OnTick(double DeltaTime);

	Camera* GetCamera();
	void SetCamera(Camera* camera);

	Ref<Texture> GetSkybox();
	void SetSkybox(Ref<Texture> skybox);

private:
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void AddLight(Light* light);
	void RemoveLight(Light* light);

};