#include "Scene.h"

#include "Scene/Entity.h"
#include "Scene/EntityComponent.h"

#include "Scene/Components/MeshComponent.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Core/Application.h"

#include "Asset/AssetModule.h"

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

Scene* Scene::Create(std::string file)
{
	Scene* scene = new Scene();

	Ref<Config> config = Config::Load(file);

	auto& types = (*config)["defaultTypes"];
	for (int i = 0; i < types.Count; i++)
	{
		EntityTypeManager::Load(types[i]);
	}

	scene->LoadScene((*config)["entities"]);

	return scene;
}

void Scene::LoadScene(Config& entities)
{
	for (int i = 0; i < entities.Count; i++)
	{
		SpawnEntity(entities[i]);
	}
}

Entity* Scene::SpawnEntity(std::string type, glm::vec3 position, glm::vec3 orientation)
{
	auto entType = EntityTypeManager::Get<EntityType>(type);
	return SpawnEntity<Entity>(entType, position, orientation);
}

Entity* Scene::SpawnEntity(Config& config)
{
	glm::vec3 position = config["position"].Vec3();
	glm::vec3 orientation = config["orientation"].Vec3();
	Entity* entity = SpawnEntity(config["type"].String, position, orientation);

	auto& children = config["children"];
	if (!children.IsNull())
	{
		for (int i = 0; i < children.Count; i++)
		{
			entity->AddChild(SpawnEntity(children[i]));
		}
	}

	return entity;
}

void Scene::OnStart()
{
}

void Scene::OnStop()
{
}

void Scene::OnSimulate(double DeltaTime, Renderer* Renderer)
{
	auto camera = GetCamera();
	glm::dvec3 cameraPos(0.0f);
	if (camera)
	{
		cameraPos = camera->GetPosition();
		auto up = camera->GetUpDirection();
		auto forward = camera->GetForwardDirection();

		m_ViewMatrix = glm::lookAt(glm::vec3(cameraPos), glm::vec3(cameraPos) + forward, up);
	}

	std::vector<MeshComponent*> meshes;

	for (int i = 0; i < m_Entities.size(); i++)
	{
		meshes.clear();

		Entity* entity = m_Entities[i];
		entity->OnSimulate(DeltaTime);

		glm::dmat4 transform = entity->GetWorldTransform();

		if (entity->IsVisible()) entity->GetComponents(meshes);

		for (auto& mesh : meshes)
		{
			mesh->Submit(transform, Renderer);
		}
	}
}

Camera* Scene::GetCamera()
{
	return m_Camera;
}

void Scene::SetCamera(Camera* camera)
{
	m_Camera = camera;
}

glm::mat4& Scene::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

void Scene::SetProjectionMatrix(glm::mat4 transform)
{
	m_ProjectionMatrix = transform;
}

glm::mat4& Scene::GetViewMatrix()
{
	return m_ViewMatrix;
}

Ref<Texture> Scene::GetSkybox()
{
	return m_Skybox;
}

void Scene::SetSkybox(Ref<Texture> skybox)
{
	m_Skybox = skybox;
}

void Scene::AddEntity(Entity* entity)
{
	entity->m_Scene = this;
	entity->m_ID = m_NextID++;
	m_Entities.push_back(entity);
}

void Scene::RemoveEntity(Entity* entity)
{
	auto position = std::find(m_Entities.begin(), m_Entities.end(), entity);
	if (position != m_Entities.end())
	{
		m_Entities.erase(position);
	}
}

void Scene::AddLight(Light* light)
{
	m_Lights.push_back(light);
}

void Scene::RemoveLight(Light* light)
{
	auto position = std::find(m_Lights.begin(), m_Lights.end(), light);
	if (position != m_Lights.end())
	{
		m_Lights.erase(position);
	}
}
