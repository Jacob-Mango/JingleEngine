#include "Scene.h"

#include "Entity.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "Application.h"
#include "Binding.h"

#include "../Asset/AssetManager.h"

#include "../Rendering/Camera.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Shader.h"

Scene::Scene(Application* app) : m_Application(app)
{
	m_Frustum = new Frustum();
}

Scene::~Scene()
{
}

void Scene::LoadScene(Config& entities)
{
	for (int i = 0; i < entities.Count; i++)
	{
		SpawnEntity(entities[i]);
	}
}

Ref<Entity> Scene::SpawnEntity(std::string type, glm::vec3 position, glm::vec3 orientation)
{
	Ref<EntityType> entType = m_Application->GetEntityType(type);
	if (entType.IsNull())
		return nullptr;

	Ref<Entity> entity = entType->Create(this, position, orientation);

	return entity;
}

Ref<Entity> Scene::SpawnEntity(Config& config)
{
	auto strToVec3 = [this](Config& config)
	{
		glm::vec3 vec = { 0, 0, 0 };

		if (!config.IsNull())
		{
			std::istringstream stream(config.String);

			stream >> vec.x;
			stream >> vec.y;
			stream >> vec.z;
		}

		return vec;
	};

	glm::vec3 position = strToVec3(config["position"]);
	glm::vec3 orientation = strToVec3(config["orientation"]);
	Ref<Entity> entity = SpawnEntity(config["type"].String, position, orientation);

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
	ImGui::Begin("Another Window");

	auto camera = GetCamera();
	glm::dvec3 cameraPos(0.0f);
	if (camera)
	{
		cameraPos = camera->GetPosition();
		auto up = camera->GetUpDirection();
		auto forward = camera->GetForwardDirection();

		m_ViewMatrix = glm::lookAt(glm::vec3(cameraPos), glm::vec3(cameraPos) + forward, up);
	}

	m_Frustum->Update(cameraPos, m_ViewMatrix);

	m_Application->DrawText("Num Entities: " + m_Entities.size());
	for (int i = 0; i < m_Entities.size(); i++)
	{
		Ref<Entity> entity = m_Entities[i];

		entity->OnSimulate(DeltaTime);

		std::string entStr = "[" + std::to_string(i) + "]: " + entity->AsString();
		ImGui::Text(entStr.c_str());

		if (!entity->IsVisible())
			continue;

		Ref<Mesh> mesh = entity->GetMesh();
		if (!mesh)
			continue;

		glm::mat4 transform = entity->GetWorldTransform();

		Renderer->SubmitStaticMesh(mesh, transform);
	}

	ImGui::End();
}

Ref<Camera> Scene::GetCamera()
{
	return m_Camera;
}

void Scene::SetCamera(Ref<Camera> camera)
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

Frustum* Scene::GetFrustum()
{
	return m_Frustum;
}

Ref<Texture> Scene::GetSkybox()
{
	return m_Skybox;
}

void Scene::SetSkybox(Ref<Texture> skybox)
{
	m_Skybox = skybox;
}

void Scene::AddEntity(Ref<Entity> entity)
{
	entity->m_ID = m_NextID++;
	m_Entities.push_back(entity);
}

void Scene::RemoveEntity(Ref<Entity> entity)
{
	auto position = std::find(m_Entities.begin(), m_Entities.end(), entity);
	if (position != m_Entities.end())
	{
		m_Entities.erase(position);
	}
}

void Scene::AddLight(Ref<Light> light)
{
	m_Lights.push_back(light);
}

void Scene::RemoveLight(Ref<Light> light)
{
	auto position = std::find(m_Lights.begin(), m_Lights.end(), light);
	if (position != m_Lights.end())
	{
		m_Lights.erase(position);
	}
}
