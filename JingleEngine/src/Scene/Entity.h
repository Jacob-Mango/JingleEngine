#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

#include "Rendering/Material.h"
#include "Rendering/MeshInstance.h"

class Scene;
class Config;
class Entity;
class Component;
class Application;

class EntityFile : public Asset
{
	SETUP_ASSET(EntityFile, Asset)

	friend Scene;

	Ref<Config> m_Config;

public:
	EntityFile();
	virtual ~EntityFile();

	virtual bool OnLoad() override;

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

	std::unordered_map<JingleScript::Type*, std::vector<Component*>> m_Components;

	bool m_IsDeleting = false;

protected:
	bool m_IsVisible = true;

	glm::dmat4 m_Transform = glm::dmat4(1.0);
	glm::vec3 m_BoundingBox[2];

public:
	Entity();
	virtual ~Entity();

	void AddComponent(Component* component);

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

	glm::dvec3 GetRightDirection() const;
	glm::dvec3 GetUpDirection() const;
	glm::dvec3 GetForwardDirection() const;

	Scene* GetScene() const;

	void SetVisible(bool visible);
	bool IsVisible();
	
	virtual std::string ToString() const override;

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnTick(double DeltaTime);
};
