#pragma once

#include <map>
#include <vector>

#include "Core.h"
#include <JingleScript.h>

#include "Shader.h"
#include "Mesh.h"

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

	Ref<Scene> m_Scene;

public:
	Renderer() {}

	void SubmitStaticMesh(Ref<Mesh> mesh, glm::mat4 transform);

	virtual void OnTick(double DeltaTime) override;
};
