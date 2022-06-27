#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Rendering/Material.h"
#include "Rendering/MeshInstance.h"

#include "Property/Serializable.h"

class Config;
class Entity;
class Component;
class Application;

class Renderer;

class EntityPropertiesPanel;
class SceneHierarchyPanel;

class ComponentArray : public JingleScript::Array, public std::vector<Component*>
{
	DEFINE_BASE_STRUCTURE(ComponentArray, JingleScript::Array);

public:
	void Insert(Component* value);

};

class EntityArray : public JingleScript::Array, public std::vector<Entity*>
{
	DEFINE_BASE_STRUCTURE(EntityArray, JingleScript::Array);

public:
	void Insert(Entity* value);

};

class Entity : public JingleScript::Object, public Serializable
{
	DEFINE_CLASS(Entity, JingleScript::Object);

	friend Renderer;

	friend EntityPropertiesPanel;
	friend SceneHierarchyPanel;

private:
	Entity* m_Parent = nullptr;
	EntityArray m_Children;

	ComponentArray m_Components;

	bool m_IsDeleting = false;

protected:
	bool m_IsVisible = true;

	glm::dmat4 m_Transform = glm::dmat4(1.0);
	glm::vec3 m_BoundingBox[2];

public:
	Entity();
	virtual ~Entity();

	void AddComponent(Component* component);
	ComponentArray& GetComponents();

	void Delete();
	bool IsDeleting();

	void AddChild(Entity* child);
	void RemoveChild(Entity* child);
	Entity* GetParent();

	virtual void SetTransform(glm::dmat4 transform);
	glm::dmat4 GetTransform();

	glm::dmat4 GetWorldTransform();

	void SetPosition(glm::dvec3 position);
	glm::dvec3 GetPosition() const;

	void SetOrientation(glm::vec3 orientation);
	glm::vec3 GetOrientation() const;

	glm::dvec3 GetRightDirection() const;
	glm::dvec3 GetUpDirection() const;
	glm::dvec3 GetForwardDirection() const;

	void SetVisible(bool visible);
	bool IsVisible();
	
	virtual std::string ToString() const override;

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnTick(double DeltaTime);

private:
	void InitChild(Entity* child);

public:
	static Entity* Create(AssetID asset);

};
