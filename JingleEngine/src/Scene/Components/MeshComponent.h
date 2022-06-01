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
	glm::dmat4 m_Transform = glm::dmat4(1.0);

public:
	MeshComponent() {}

	void Submit(const glm::dmat4& InTransform, Renderer* Renderer);

public:
	inline void SetMesh(MeshInstance* InMesh) { m_Mesh = InMesh; }
	inline MeshInstance* GetMesh() const { return m_Mesh; }
	
	inline void SetTransform(const glm::dmat4& InTransform) { m_Transform = InTransform; }
	inline glm::dmat4 GetTransform() const { return m_Transform; }

};
