#pragma once

#include "Scene/Entity.h"

#include "Core/Application.h"
#include "Core/Config.h"

class Camera;
class Texture;
class Light;

class Scene : public Countable
{
public:
	friend class Entity;
	friend class Light;

	friend class Renderer;

private:
	std::vector<Ref<Entity>> m_Entities;
	std::vector<Ref<Light>> m_Lights;

	unsigned int m_NextID = 0;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	Ref<Texture> m_Skybox;

	Ref<Camera> m_Camera;

public:
	void LoadScene(Config& entities);

	Ref<Entity> SpawnEntity(std::string type, glm::vec3 position, glm::vec3 orientation);
	Ref<Entity> SpawnEntity(Config& entity);

	void OnStart();
	void OnStop();
	void OnSimulate(double DeltaTime, class Renderer* Renderer);

	template <typename T>
	Ref<T> SpawnEntity(glm::vec3 position = glm::vec3(0), glm::vec3 orientation = glm::vec3(0))
	{
		Ref<T> ent = new T();
		ent->SetScene(this);

		AddEntity(ent.As<Entity>());

		ent->SetOrientation(orientation);
		ent->SetPosition(position);

		ent->OnCreate();

		return ent;
	}

	template <typename T, typename T2>
	Ref<T> SpawnEntity(T2* type, glm::vec3 position = glm::vec3(0), glm::vec3 orientation = glm::vec3(0))
	{
		Ref<T> ent = new T(type);
		ent->SetScene(this);

		AddEntity(ent.As<Entity>());

		ent->SetOrientation(orientation);
		ent->SetPosition(position);

		ent->OnCreate();

		return ent;
	}

	Ref<Camera> GetCamera();
	void SetCamera(Ref<Camera> camera);

	glm::mat4& GetProjectionMatrix();
	void SetProjectionMatrix(glm::mat4 transform);

	glm::mat4& GetViewMatrix();

	Ref<Texture> GetSkybox();
	void SetSkybox(Ref<Texture> skybox);

private:
	void AddEntity(Ref<Entity> entity);
	void RemoveEntity(Ref<Entity> entity);

	void AddLight(Ref<Light> light);
	void RemoveLight(Ref<Light> light);
};