#pragma once

#include <map>
#include <vector>

#include "Core.h"
#include "Core/Reference.h"

#include "Shader.h"
#include "Mesh.h"

class Renderer
{
private:
	struct MeshCommand
	{
		Ref<Shader> m_Shader;
		Ref<Mesh> m_Mesh;
		glm::mat4 m_Transform;
	};

	std::map<Shader*, std::vector<MeshCommand>> m_StaticMeshes;

public:
	void SubmitStaticMesh(Ref<Mesh> mesh, glm::mat4 transform);
	void Render(class Scene* Scene);
};
