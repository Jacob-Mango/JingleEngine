#include "Scene.h"

#include "Asset/AssetModule.h"

#include "Core/Application.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"

#include "Scene/Components/MeshComponent.h"

#include "Property/PropertyArray.h"

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

//! TODO: Seperate 'SceneData' from 'Scene'
BEGIN_CLASS_LINK(Scene)
	LINK_NAMED_VARIABLE(Entities, m_EntitiesData);
	LINK_CONSTRUCTOR();
	LINK_METHOD(GetCamera);
END_CLASS_LINK()

Scene::Scene()
{
	JS_TRACE(Tracers::Asset);

	m_EntitiesData = nullptr;
}

Scene::~Scene()
{

}

bool Scene::OnLoad()
{
	JS_TRACE(Tracers::Asset);

	using namespace JingleScript;

	if (!Super::OnLoad())
	{
		return false;
	}

	JS_TINFO("Instance: {}", PointerToString(this));

	for (int i = 0; i < m_EntitiesData->Count(); i++)
	{
		const auto& property = m_EntitiesData->Get(i);

		Type* type = property->GetPropertyType();
		Entity* entity = type->New<Entity>();
		
		property->OnWriteObject(entity);
		
		AddEntity(entity);
		entity->m_Parent = nullptr;
		
		for (auto& component : entity->GetComponents())
		{
			component->m_Entity = entity;
			component->OnCreate();
		}
		
		entity->OnCreate();
	}

	return true;
}

void Scene::OnStart()
{
}

void Scene::OnStop()
{
}

void Scene::OnTick(double DeltaTime)
{
	for (int i = 0; i < m_Entities.size(); i++)
	{
		Entity* entity = m_Entities[i];

		entity->OnTick(DeltaTime);
				
		for (auto& component : entity->GetComponents())
		{
			component->OnTick(DeltaTime);
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
