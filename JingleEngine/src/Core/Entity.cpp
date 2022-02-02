#include "Entity.h"

#include <glm/gtx/euler_angles.hpp>

#include "Scene.h"
#include "Application.h"

#include "../Asset/AssetManager.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"

void EntityType::Register(Application* app)
{
	app->RegisterBaseEntityType(BaseName(), new EntityType());
}

Ref<Entity> EntityType::Create(Ref<Scene> scene)
{
	return Create(scene, glm::vec3(0), glm::vec3(0));
}

Ref<Entity> EntityType::Create(Ref<Scene> scene, glm::vec3 position, glm::vec3 orientation)
{
	return scene->SpawnEntity<Entity>(this, position, orientation);
}

void EntityType::Load(Config& config)
{
	Name = config.GetName();

	std::string model = config["model"].String;
	if (std::strcmp(model.c_str(), "") != 0)
	{
		Model = AssetManager::Get<MeshAsset>(model);
	}

	std::string material = config["material"].String;
	if (std::strcmp(material.c_str(), "") != 0)
	{
		Material = AssetManager::Get<::Material>(material);
	}
}

Entity::Entity(EntityType* type) : m_Type(type), m_Transform(1.0)
{
	m_BoundingBox[0] = glm::vec3(-1);
	m_BoundingBox[1] = glm::vec3(1);

	if (!GetType()->Model.IsNull())
	{
		m_Mesh = new Mesh(GetType()->Model);

		if (!GetType()->Material.IsNull())
		{
			m_Mesh->SetMaterial(GetType()->Material);
		}
	}
}

Entity::~Entity()
{
	if (!m_IsDeleting)
	{
		std::cerr << "Entity being deleted without being called by Entity::Delete!" << std::endl;
	}
}

void Entity::AddChild(Ref<Entity> child)
{
	if (child->m_Parent != nullptr)
	{
		if (child->m_Parent == this)
			return;

		child->m_Parent->RemoveChild(child);
	}

	child->m_Parent = this;
	m_Children.push_back(child);
}

void Entity::Delete()
{
	if (m_IsDeleting) return;
	m_IsDeleting = true;

	OnDestroy();

	for (int i = m_Children.size() - 1; i >= 0; i--)
	{
		//RemoveChild(m_Children[i]);
	}

	m_Scene->RemoveEntity(this);
}

bool Entity::IsDeleting()
{
	return m_IsDeleting;
}

void Entity::RemoveChild(Ref<Entity> child)
{
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] != child)
			continue;

		child->m_Parent = nullptr;
		m_Children.erase(m_Children.begin() + i);
	}
}

Ref<Entity> Entity::GetParent()
{
	return m_Parent;
}

void Entity::SetTransform(glm::dmat4 transform)
{
	m_Transform = transform;
}

glm::dmat4 Entity::GetTransform()
{
	return m_Transform;
}

glm::dmat4 Entity::GetWorldTransform()
{
	glm::dmat4 transform(1.0);

	Entity* parent = this;
	while (parent != nullptr)
	{
		transform = parent->GetTransform() * transform;
		parent = parent->GetParent();
	}

	return transform;
}

void Entity::SetPosition(glm::dvec3 position)
{
	m_Transform[3][0] = position[0];
	m_Transform[3][1] = position[1];
	m_Transform[3][2] = position[2];
	SetTransform(m_Transform);
}

glm::dvec3 Entity::GetPosition()
{
	return glm::vec3(m_Transform[3]);
}

glm::vec3 Entity::GetRightDirection()
{
	return glm::vec3(m_Transform[0]);
}

glm::vec3 Entity::GetUpDirection()
{
	return glm::vec3(m_Transform[1]);
}

glm::vec3 Entity::GetForwardDirection()
{
	return glm::vec3(m_Transform[2]);
}

void Entity::SetOrientation(glm::vec3 orientation)
{
	glm::dmat4 trans = glm::eulerAngleYXZ(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
	trans[3][0] = m_Transform[3][0];
	trans[3][1] = m_Transform[3][1];
	trans[3][2] = m_Transform[3][2];
	SetTransform(trans);
}

glm::vec3 Entity::GetOrientation()
{
	glm::vec3 orientation = glm::vec3();
	glm::extractEulerAngleYXZ(glm::mat4(m_Transform), orientation.x, orientation.y, orientation.z);
	return glm::degrees(orientation);
}

void Entity::SetScene(Scene* Scene)
{
	m_Scene = Scene;
}

Scene* Entity::GetScene()
{
	return m_Scene;
}

Ref<EntityType> Entity::GetType()
{
	return m_Type;
}

Ref<Mesh> Entity::GetMesh()
{
	return m_Mesh;
}

std::string EntityType::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

	ss << ", ";
	ss << "Name=" << Name;

	return ss.str();
}

std::string Entity::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

	ss << ", ";
	ss << "Type=" << m_Type.AsString();

	ss << ", ";
	ss << "Position=" << GetPosition();

	ss << ", ";
	ss << "Orientation=" << GetOrientation();

	ss << ", ";
	ss << "Mesh=" << m_Mesh.AsString();

	return ss.str();
}

void Entity::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

bool Entity::IsVisible()
{
	return m_IsVisible;
}

void Entity::OnCreate()
{
}

void Entity::OnDestroy()
{
}

void Entity::OnSimulate(double DeltaTime)
{
}
