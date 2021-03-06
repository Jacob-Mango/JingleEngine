#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Rendering/Material.h"
#include "Rendering/MeshInstance.h"

class Config;
class Entity;
class Component;
class Application;

class Renderer;

class EntityPropertiesPanel;
class EntityHierarchyPanel;

class EntityArray;
class ComponentArray;

class Entity : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Entity, JingleScript::Object);

	friend Renderer;

	friend EntityPropertiesPanel;
	friend EntityHierarchyPanel;

private:
	Entity* m_Parent = nullptr;
	EntityArray* m_Children = nullptr;

	glm::dmat4 m_Transform = glm::dmat4(1.0);
	glm::vec3 m_BoundingBox[2];

	ComponentArray* m_Components = nullptr;

	bool m_IsDeleting = false;
	bool m_IsVisible = true;

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

	virtual void Editor_OnPropertyChanged(std::string name) override;

private:
	void OnSerializeTransform(Config* cfg);
	void OnDeserializeTransform(Config* cfg);
	void Editor_OnRenderTransform();

	void OnSerializeComponents(Config* cfg);
	void OnDeserializeComponents(Config* cfg);
	void Editor_OnRenderComponents();

	void OnSerializeChildren(Config* cfg);
	void OnDeserializeChildren(Config* cfg);
	void Editor_OnRenderChildren();

public:
	static Entity* Create(AssetID asset);

};

class EntityArray : public JingleScript::Array<Entity*>
{
	DEFINE_CLASS(EntityArray, JingleScript::Array<Entity*>);

public:
	EntityArray() {}
	~EntityArray() {}

};
