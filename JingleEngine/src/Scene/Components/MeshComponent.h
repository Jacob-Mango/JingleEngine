#pragma once

#include "Asset/AssetID.h"

#include "Core/Core.h"

#include "Scene/Component.h"

class MeshInstance;
class Renderer;

class MeshComponent : public Component
{
	DEFINE_CLASS(MeshComponent, Component);

public:
	std::string Model;
	std::string Material;

private:
	Ref<MeshInstance> m_Mesh;

public:
	MeshComponent() {}
	MeshComponent(Entity* entity) : Super(entity) {}

	virtual void OnCreate() override;

public:
	MeshInstance* GetMesh() const { return m_Mesh; }

};
