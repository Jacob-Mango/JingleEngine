#include "Scene.h"

#include "Asset/AssetModule.h"

#include "Core/Application.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"

#include "Scene/Components/MeshComponent.h"

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

BEGIN_CLASS_LINK(Scene)
	LINK_CONSTRUCTOR();
	LINK_METHOD(GetCamera);
END_CLASS_LINK()

Scene* Scene::Create(std::string file)
{
	Scene* scene = JingleScript::NewObject<Scene>("Scene");

	//Ref<Config> config = Config::Load(file);

	//scene->LoadScene((*config)["entities"]);

	return scene;
}

void Scene::LoadScene(Config& entities)
{
	/*
	for (int i = 0; i < entities.Count(); i++)
	{
		auto& config = entities.Get(i);

		auto position = config["position"].Vec3();
		auto orientation = config["orientation"].Vec3();
		auto file = config["file"].String;

		auto entityFile = AssetModule::Get<EntityFile>(file);

		Config* entityFileConfig = entityFile->m_Config.Get();
		
		Entity* entity = SpawnEntity((*entityFileConfig)[0]);
		if (!entity)
		{
			continue;
		}

		entity->SetPosition(position);
		entity->SetOrientation(orientation);
	}
		*/
}

/*
Entity* Scene::SpawnEntity(std::string type, glm::vec3 position, glm::vec3 orientation)
{
	//auto entType = EntityTypeManager::Get<EntityType>(type);
	//return SpawnEntity<Entity>(entType, position, orientation);
	return nullptr;
}
*/

Entity* Scene::SpawnEntity(Config& cfgRoot, Entity* parent)
{
	/*
	using namespace JingleScript;

	auto parseConfig = [](Config& cfg, Object* object, Type* type)
	{
		auto variables = type->GetVariables();

		for (auto& variable : variables)
		{
			Property* prop = nullptr;
			for (auto& attributeBase : variable->Attributes)
			{
				if (attributeBase->GetType() != Property::StaticType())
				{
					continue;
				}

				prop = static_cast<Property*>(attributeBase);
			}

			if (prop == nullptr)
			{
				continue;
			}

			void* src = nullptr;
			void* dst = object->GetVariable(variable->Name);

			if (variable->Type->IsStructure())
			{
				double dbl;

				if (variable->Type == Integer::StaticType())
				{
					src = (void*)&(cfg[variable->Name].Int);
				}
				else if (variable->Type == Float::StaticType())
				{
					src = (void*)&(cfg[variable->Name].Float);
				}
				else if (variable->Type == Double::StaticType())
				{
					dbl = cfg[variable->Name].Float;
					src = (void*)&(dbl);
				}
				else if (variable->Type == String::StaticType())
				{
					src = (void*)&(cfg[variable->Name].String);
				}

				Type::CallCopyConstructor(src, dst, variable->Type);
			}
			else
			{
				// not supported yet.
				// spawn the class and assign the data
			}
		}
	};

	Type* type = TypeManager::Get(cfgRoot.GetName());

	Entity* entity = type->New<Entity>();
	entity->m_Parent = parent;
	AddEntity(entity);

	parseConfig(cfgRoot, entity, type);

	auto& cfgComponents = cfgRoot["Components"];
	for (int i = 0; i < cfgComponents.Count; i++)
	{
		auto& cfgComponent = cfgComponents[i];

		Type* componentType = TypeManager::Get(cfgComponent.GetName());

		Component* component = componentType->New<Component>(entity);

		entity->AddComponent(component);

		parseConfig(cfgComponent, component, componentType);
	}

	std::vector<Component*> components;
	entity->GetComponents(components);

	for (auto& component : components)
	{
		component->OnCreate();
	}

	entity->OnCreate();

	return entity;
		*/
		return nullptr;
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

		std::vector<Component*> components;
		entity->GetComponents(components);

		for (auto& component : components)
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
