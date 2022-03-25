#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

class MeshEntity;
class MeshEntityType : public EntityType
{
	DEFINE_CLASS(MeshEntityType, EntityType)

public:
	Ref<MeshAsset> Model;
	Ref<Material> Material;

public:
	MeshEntityType() {}

	virtual void Load(Config& config) override;

};

class MeshEntity : public Entity
{
	DEFINE_CLASS(MeshEntity, Entity);

public:
	MeshEntity() {}

	Ref<Mesh> GetMesh() const;
	
	virtual void OnCreate() override;

	virtual std::string ToString() const override;

};
