#pragma once

#include "Core/Core.h"

class Scene;
class Config;
class Entity;
class EntityComponent;
class Application;

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"

class EntityType : public JingleScript::Object
{
	DEFINE_CLASS(EntityType, JingleScript::Object);

public:
	std::string Name;

public:
	EntityType() {}

	virtual void Load(Config& config);
	
	virtual std::string ToString() const override;
};

class Entity : public JingleScript::Object
{
	DEFINE_CLASS(Entity, JingleScript::Object);

	friend Scene;

private:
	unsigned int m_ID;

	Scene* m_Scene;

	Entity* m_Parent;
	std::vector<Entity*> m_Children;

	std::unordered_map<JingleScript::Type*, std::vector<EntityComponent*>> m_Components;

	bool m_IsDeleting = false;

	EntityType* m_EntityType;

protected:
	bool m_IsVisible = true;

	glm::dmat4 m_Transform;
	glm::vec3 m_BoundingBox[2];

public:
	Entity();
	virtual ~Entity();

	void AddComponent(EntityComponent* component);

	template<typename T>
	T* AddComponent()
	{
		T* component = JingleScript::NewObject<T>(T::StaticName());
		AddComponent(component);
		return component;
	}

	/**
	 * Get all the components derived from 'T'
	 * 
	 * @param outComponents		
	 */
	template<typename T>
	void GetComponents(std::vector<T*>& outComponents)
	{
		auto& components = m_Components[T::StaticType()];
		outComponents.reserve(outComponents.size() + components.size());
		for (auto& component : components)
			outComponents.push_back(static_cast<T*>(component));
	}

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

	glm::vec3 GetRightDirection() const;
	glm::vec3 GetUpDirection() const;
	glm::vec3 GetForwardDirection() const;

	Scene* GetScene() const;

	EntityType& GetEntityType() const;

	template<typename T>
	T& GetEntityType() const
	{
		return *(dynamic_cast<T*>(m_EntityType));
	}

	void SetVisible(bool visible);
	bool IsVisible();
	
	virtual std::string ToString() const override;

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnSimulate(double DeltaTime);
};
