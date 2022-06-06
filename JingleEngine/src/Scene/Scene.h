#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Config.h"

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

private:
	std::vector<Entity*> m_Entities;
	std::vector<Light*> m_Lights;

	unsigned int m_NextID = 0;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	Ref<Texture> m_Skybox;

	Camera* m_Camera;

private:
	Scene() {}

public:
	virtual ~Scene() {}

	static Scene* Create(std::string file);

	void LoadScene(Config& entities);

	Entity* SpawnEntity(std::string type, glm::vec3 position, glm::vec3 orientation);
	Entity* SpawnEntity(Config& entity);

	void OnStart();
	void OnStop();
	void OnSimulate(double DeltaTime, class Renderer* Renderer);

	template <typename T>
	T* SpawnEntity(EntityType* entityType, glm::vec3 position = glm::vec3(0), glm::vec3 orientation = glm::vec3(0))
	{
		using namespace JingleScript;

		Type* type = TypeManager::Get(entityType->Name);
		if (type == nullptr)
		{
			JS_ERROR("Invalid entity type");
			return nullptr;
		}

		T* entity = type->InternalAllocate<T>();
		if (!entity)
		{
			JS_ERROR("Failed to allocate entity");
			return nullptr;
		}

		entity->m_EntityType = entityType;

		AddEntity(entity);

		Type::CallDefaultConstructor(entity, type);

		entity->m_Parent = nullptr;

		entity->SetOrientation(orientation);
		entity->SetPosition(position);

		type->InitializeScript(entity);

		entity->OnCreate();

		return entity;
	}

	Camera* GetCamera();
	void SetCamera(Camera* camera);

	glm::mat4& GetProjectionMatrix();
	void SetProjectionMatrix(glm::mat4 transform);

	glm::mat4& GetViewMatrix();

	Ref<Texture> GetSkybox();
	void SetSkybox(Ref<Texture> skybox);

private:
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void AddLight(Light* light);
	void RemoveLight(Light* light);

};