#include "Renderer.h"

#include "Rendering/Viewport.h"

#include "Scene/Scene.h"

#include "Scene/Components/MeshComponent.h"

#include "Core/ModuleManager.h"
#include "Core/Window.h"

BEGIN_MODULE_LINK(Renderer);
END_MODULE_LINK();

void Renderer::SubmitStaticMesh(Ref<MeshInstance> mesh, glm::dmat4 transform)
{
	if (!mesh) return;

	Ref<Material> material = mesh->GetMaterial();

	if (!material) return;

	Ref<Shader> shader = material->GetShader();

	if (!shader) return;

	m_StaticMeshes[shader].push_back({ shader, mesh, transform });
}

//! TODO: Refactor and abstract
void Renderer::Process(double DeltaTime, Viewport* viewport)
{
	m_StaticMeshes.clear();

	Scene* scene = viewport->m_Scene;

	for (auto& entity : scene->m_Entities)
	{
		if (!entity->IsVisible())
		{
		//	continue;
		}
		
		glm::dmat4 transform = entity->GetWorldTransform();

		auto& components = entity->GetComponents();
		for (auto& component : components)
		{
			MeshComponent* comp = dynamic_cast<MeshComponent*>(component);
			if (comp)
			{
				SubmitStaticMesh(comp->GetMesh(), transform);
			}
		}
	}

	GL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
	GL(glEnable(GL_DEPTH_TEST));
	GL(glEnable(GL_CULL_FACE));

	GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	GL(glClearColor(0.0, 0.0, 0.0, 1.0));
	GL(glClearDepth(1.0f));

	GL(glCullFace(GL_BACK));
	GL(glFrontFace(GL_CCW));

	GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GL(glViewport(0, 0, viewport->m_Width, viewport->m_Height));

	for (auto& [shader, commands] : m_StaticMeshes)
	{
		shader->Bind();

		shader->Set("u_ProjectionMatrix", viewport->m_ProjectionMatrix);
		shader->Set("u_ViewMatrix", viewport->m_ViewMatrix);
		shader->Set("u_CameraPosition", viewport->m_CameraPosition);

		int numDirectionalLights = 0;
		int numPointLights = 0;

		if (scene)
		{
			int index = 0;
			for (auto& light : scene->m_Lights)
			{
				light->Process(shader, numPointLights, numDirectionalLights);

				index++;
			}
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
}
