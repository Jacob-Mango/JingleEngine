#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

class MeshEntity;
class MeshEntityType : public EntityType
{
	ENTITY_TYPE(MeshEntity, Entity)
public:

	Ref<MeshAsset> Model;
	Ref<Material> Material;
};

class MeshEntity : public Entity
{
	ENTITY(MeshEntity, Entity)

protected:

	Ref<Mesh> m_Mesh;

public:
	Ref<Mesh> GetMesh();
	
	virtual void OnCreate() override;

	virtual std::string ToString() override;

};
