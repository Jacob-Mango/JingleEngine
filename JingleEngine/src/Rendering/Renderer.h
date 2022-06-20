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
		Ref<Shader> m_Shader;
		MeshInstance* m_Mesh;
		glm::mat4 m_Transform;
	};

	std::map<Shader*, std::vector<MeshCommand>> m_StaticMeshes;

public:
	void SubmitStaticMesh(Ref<MeshInstance> mesh, glm::dmat4 transform);

	void Process(double DeltaTime, class Viewport* viewport);

};
