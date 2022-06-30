#pragma once

#include "Asset/AssetID.h"

#include "Core/Core.h"

#include "Entities/Component.h"

class MeshInstance;
class Renderer;

class MeshComponent : public Component
{
	DEFINE_CLASS(MeshComponent, Component);

public:
	Mesh* Model = nullptr;
	Material* MaterialOverride = nullptr;

private:
	Ref<MeshInstance> m_Mesh;

public:
	MeshComponent() {}
	MeshComponent(Entity* entity) : Super(entity) {}

	virtual void OnCreate() override;

	virtual void Editor_OnPropertyChanged(std::string name) override;

public:
	MeshInstance* GetMesh() const { return m_Mesh; }

};
