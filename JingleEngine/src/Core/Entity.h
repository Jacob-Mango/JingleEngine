#pragma once

#include "Reference.h"

#include "Core.h"

class Scene;

#define ENTITY_TYPE(T, BASE)                                											\
	public:                                																\
	typedef BASE##Type super;                               											\
	virtual Ref<Entity> Create(Ref<Scene> scene, glm::vec3 position, glm::vec3 orientation) override	\
	{																									\
		return scene->SpawnEntity<##T##>(this, position, orientation).As<Entity>();						\
	}																									\
	virtual void Load(Config& config) override;															\
	virtual EntityType* CreateType() override { return new T##Type(); }									\
	static std::string BaseName() { return std::string(#T) + "Type"; }									\
	static void Register(Application* app) {															\
		app->RegisterBaseEntityType(BaseName(), new T##Type());											\
	}																									\
	private:

#define ENTITY(T, BASE)                                						\
	public:                                									\
	typedef BASE super;														\
	Ref<##T##Type> GetType()												\
	{																		\
		return BASE::GetType().As<##T##Type>();								\
	}																		\
	static Ref<##T##Type> StaticGetType(Ref<EntityType> type)				\
	{																		\
		return type.As<##T##Type>();										\
	}																		\
	T() : T(new T##Type()) {}												\
	T(T##Type* type) : BASE(type) {}										\
	private:

class Scene;
class Config;
class Entity;
class Application;

#include "../Rendering/Material.h"
#include "../Rendering/Mesh.h"

class EntityType : public Countable
{
	typedef Countable base;

public:
	std::string Name;

	Ref<MeshAsset> Model;
	Ref<Material> Material;

	static std::string BaseName() { return "EntityType"; }
	static void Register(class Application* app);

	Ref<Entity> Create(Ref<Scene> scene);
	virtual Ref<Entity> Create(Ref<Scene> scene, glm::vec3 position, glm::vec3 orientation);

	virtual EntityType* CreateType() { return new EntityType(); }

	virtual void Load(Config& config);
	
	virtual std::string ToString() override;
};

#define ENTITY_CONSTRUCTOR(T) T(Ref<EntityType> type) : Entity(type) {}

class Entity : public Countable
{
	friend Scene;

	typedef Countable base;

private:
	unsigned int m_ID = 0;

	Scene* m_Scene = nullptr;

	Ref<Entity> m_Parent;
	std::vector<Entity*> m_Children;

	bool m_IsDeleting = false;

	Ref<EntityType> m_Type;

protected:
	bool m_IsVisible = true;

	Ref<Mesh> m_Mesh;
	glm::dmat4 m_Transform;
	glm::vec3 m_BoundingBox[2];

public:
	Entity(EntityType* type);
	virtual ~Entity();

	void Delete();
	bool IsDeleting();

	void AddChild(Ref<Entity> child);
	void RemoveChild(Ref<Entity> child);
	Ref<Entity> GetParent();

	virtual void SetTransform(glm::dmat4 transform);
	glm::dmat4 GetTransform();

	glm::dmat4 GetWorldTransform();

	void SetPosition(glm::dvec3 position);
	glm::dvec3 GetPosition();

	void SetOrientation(glm::vec3 orientation);
	glm::vec3 GetOrientation();

	glm::vec3 GetRightDirection();
	glm::vec3 GetUpDirection();
	glm::vec3 GetForwardDirection();

	void SetScene(Scene* Scene);
	Scene* GetScene();

	Ref<EntityType> GetType();
	Ref<Mesh> GetMesh();

	void SetVisible(bool visible);
	bool IsVisible();
	
	virtual std::string ToString() override;

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnSimulate(double DeltaTime);
};
