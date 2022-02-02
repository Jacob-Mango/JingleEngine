#include "Renderer.h"

#include "../Core/Scene.h"

void Renderer::SubmitStaticMesh(Ref<Mesh> mesh, glm::mat4 transform)
{
	Ref<Shader> shader = mesh->GetMaterial()->GetShader();

	m_StaticMeshes[shader].push_back({ shader, mesh, transform });
}

void Renderer::Render(Scene* Scene)
{
	for (auto& [shader, commands] : m_StaticMeshes)
	{
		shader->Bind();

		shader->Set("u_ProjectionMatrix", Scene->GetProjectionMatrix());
		shader->Set("u_ViewMatrix", Scene->GetViewMatrix());
		shader->Set("u_CameraPosition", Scene->GetCamera() ? glm::vec3(Scene->GetCamera()->GetPosition()) : glm::vec3(0));

		int numDirectionalLights = 0;
		int numPointLights = 0;

		int index = 0;
		for (auto& light : Scene->m_Lights)
		{
			light->Process(shader, numPointLights, numDirectionalLights);

			index++;
		}

		shader->Set("u_NumPointLights", numPointLights);
		shader->Set("u_NumDirectionalLights", numDirectionalLights);

		for (auto& command : commands)
		{
			command.m_Mesh->GetMaterial()->Bind(shader);

			shader->Set("u_ModelMatrix", command.m_Transform);

			command.m_Mesh->Render();

			command.m_Mesh->GetMaterial()->Unbind(shader);
		}

		shader->Unbind();
	}

	m_StaticMeshes.clear();
}
