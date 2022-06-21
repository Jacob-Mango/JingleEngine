#pragma once

#include "Core/Core.h"
#include "Core/Application.h"

#include "Scene/Entity.h"

class Camera;
class Texture;
class Light;

class Scene : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Scene, JingleScript::ManagedObject);
	
	friend class Entity;
	friend class Light;

	friend class Renderer;

	friend class SceneHierarchyPanel;

private:
	std::vector<Entity*> m_Entities;
	std::vector<Light*> m_Lights;

	unsigned int m_NextID = 0;

	Ref<Texture> m_Skybox;

	Camera* m_Camera;

public:
	Scene() {}
	virtual ~Scene() {}

	static Scene* Create(std::string file);

	void LoadScene(Config& entities);

	//Entity* SpawnEntity(AssetID entity, glm::vec3 position, glm::vec3 orientation);
	Entity* SpawnEntity(Config& entity, Entity* parent = nullptr);

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