#include "Renderer.h"

#include "Scene/Scene.h"

#include "Core/ModuleManager.h"
#include "Core/Window.h"

BEGIN_MODULE_LINK(Renderer);
END_MODULE_LINK();

void Renderer::SubmitStaticMesh(Ref<MeshInstance> mesh, glm::dmat4 transform)
{
	Ref<Shader> shader = mesh->GetMaterial()->GetShader();

	m_StaticMeshes[shader].push_back({ shader, mesh, transform });
}

void Renderer::OnTick(double DeltaTime)
{
	Scene* scene = Application::Get()->GetScene();

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);

	auto window = ModuleManager::Get<Window>();

	auto [width, height] = window->GetSize();

	if (scene)
	{
		scene->SetProjectionMatrix(glm::perspective(glm::radians(90.0f), (GLfloat)width / (GLfloat)height, 0.001f, 1000.0f));
	}

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	for (auto& [shader, commands] : m_StaticMeshes)
	{
		shader->Bind();

		if (scene)
		{
			shader->Set("u_ProjectionMatrix", scene->GetProjectionMatrix());
			shader->Set("u_ViewMatrix", scene->GetViewMatrix());
			shader->Set("u_CameraPosition", scene->GetCamera() ? glm::vec3(scene->GetCamera()->GetPosition()) : glm::vec3(0));
		}

		int numDirectionalLights = 0;
		int numPointLights = 0;

		int index = 0;
		for (auto& light : scene->m_Lights)
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
