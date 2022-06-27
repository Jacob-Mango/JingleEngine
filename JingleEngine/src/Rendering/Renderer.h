#pragma once

#include "Core.h"

#include "Rendering/Shader.h"
#include "Rendering/MeshInstance.h"

#include <map>
#include <vector>

class Renderer : public Module
{
	DEFINE_MODULE(Renderer, Module);

private:
	struct MeshCommand
	{
		MeshInstance* m_Mesh;
		Material* m_Material;
		glm::mat4 m_Transform;
	};

	std::map<Shader*, std::vector<MeshCommand>> m_Meshes;

public:
	void SubmitEntity(Entity* entity, const glm::dmat4& parentTransform);
	void SubmitMesh(Ref<MeshInstance> mesh, const glm::dmat4& transform);
	void SubmitLight(/*TODO*/);

	void Process(double DeltaTime, class Viewport* viewport);

};
