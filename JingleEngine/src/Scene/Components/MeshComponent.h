#pragma once

#include "Core/Core.h"

#include "Scene/EntityComponent.h"

class Mesh;
class Renderer;

class MeshComponent : public EntityComponent
{
	DEFINE_CLASS(MeshComponent, EntityComponent);

private:
	//TODO: merge 'Mesh' into 'MeshComponent'
	Ref<Mesh> m_Mesh;
	glm::mat4 m_Transform;

public:
	MeshComponent() {}

	void Submit(const glm::mat4& InTransform, Renderer* Renderer);

public:
	inline void SetMesh(Mesh* InMesh) { m_Mesh = InMesh; }
	inline Mesh* GetMesh() const { return m_Mesh; }
	
	inline void SetTransform(const glm::mat4& InTransform) { m_Transform = InTransform; }
	inline glm::mat4 GetTransform() const { return m_Transform; }

};
