#pragma once

#include "Core/Core.h"

#include "Scene/EntityComponent.h"

class MeshInstance;
class Renderer;

class MeshComponent : public EntityComponent
{
	DEFINE_CLASS(MeshComponent, EntityComponent);

private:
	//TODO: Instead of MeshInstance, use a storage mesh that has both the asset and multiple 'MeshInstance' per material
	Ref<MeshInstance> m_Mesh;

public:
	MeshComponent() {}

public:
	inline void SetMesh(MeshInstance* InMesh) { m_Mesh = InMesh; }
	inline MeshInstance* GetMesh() const { return m_Mesh; }

};
