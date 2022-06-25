#include "Entity.h"

#include <glm/gtx/euler_angles.hpp>

#include "Scene/Scene.h"
#include "Scene/Component.h"

#include "Core/Application.h"

#include "Asset/AssetModule.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

BEGIN_STRUCTURE_LINK(ComponentArray)
	LINK_CONSTRUCTOR();
	LINK_METHOD(Insert);
END_STRUCTURE_LINK()

void ComponentArray::Insert(JingleScript::Object* value)
{
	push_back(static_cast<Component*>(value));
}

BEGIN_CLASS_LINK(Entity)
	LINK_NAMED_VARIABLE(Components, m_Components);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnCreate);
	LINK_METHOD(OnDestroy);
	LINK_METHOD(OnTick);
END_CLASS_LINK()

Entity::Entity()
{
	m_BoundingBox[0] = glm::vec3(-1);
	m_BoundingBox[1] = glm::vec3(1);
}

Entity::~Entity()
{
	if (!m_IsDeleting)
	{
		std::cerr << "Entity being deleted without being called by Entity::Delete!" << std::endl;
	}
}

void Entity::AddComponent(Component* component)
{
	using namespace JingleScript;

	component->m_Entity = this;
	m_Components.push_back(component);
}

ComponentArray& Entity::GetComponents()
{
	return m_Components;
}

void Entity::AddChild(Entity* child)
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

void Entity::RemoveChild(Entity* child)
{
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] != child)
			continue;

		child->m_Parent = nullptr;
		m_Children.erase(m_Children.begin() + i);
	}
}

Entity* Entity::GetParent()
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

glm::dvec3 Entity::GetPosition() const
{
	return glm::dvec3(m_Transform[3]);
}

glm::dvec3 Entity::GetRightDirection() const
{
	return glm::dvec3(m_Transform[0]);
}

glm::dvec3 Entity::GetUpDirection() const
{
	return glm::dvec3(m_Transform[1]);
}

glm::dvec3 Entity::GetForwardDirection() const
{
	return glm::dvec3(m_Transform[2]);
}

void Entity::SetOrientation(glm::vec3 orientation)
{
	glm::dmat4 trans = glm::eulerAngleYXZ(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
	trans[3] = m_Transform[3];
	SetTransform(trans);
}

glm::vec3 Entity::GetOrientation() const
{
	glm::vec3 orientation = glm::vec3();
	glm::extractEulerAngleYXZ(glm::mat4(m_Transform), orientation.x, orientation.y, orientation.z);
	return glm::degrees(orientation);
}

Scene* Entity::GetScene() const
{
	return m_Scene;
}

std::string Entity::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	ss << "Position=" << GetPosition();

	ss << ", ";
	ss << "Orientation=" << GetOrientation();

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

void Entity::OnTick(double DeltaTime)
{
}
