#pragma once

#include "Core.h"

#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"

#include <map>
#include <vector>

class Renderer : public Module
{
	DEFINE_MODULE(Renderer, Module);

private:
	struct MeshCommand
	{
		Ref<Shader> m_Shader;
		Ref<Mesh> m_Mesh;
		glm::mat4 m_Transform;
	};

	std::map<Shader*, std::vector<MeshCommand>> m_StaticMeshes;

public:
	Renderer() {}

	void SubmitStaticMesh(Ref<Mesh> mesh, glm::mat4 transform);

	virtual void OnTick(double DeltaTime) override;
};
