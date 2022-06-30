#include "Renderer.h"

#include "Rendering/Viewport.h"

#include "Entities/Entity.h"

#include "Entities/Components/MeshComponent.h"

#include "Core/ModuleManager.h"
#include "Core/Window.h"

BEGIN_MODULE_LINK(Renderer);
END_MODULE_LINK();

void Renderer::SubmitEntity(Entity* entity, const glm::dmat4& parentTransform)
{
	if (!entity->IsVisible())
	{
		return;
	}

	glm::dmat4 transform = parentTransform * entity->GetTransform();
	
	auto& components = entity->GetComponents();
	for (auto& component : components)
	{
		MeshComponent* comp = dynamic_cast<MeshComponent*>(component);
		if (comp && comp->GetMesh())
		{
			SubmitMesh(comp->GetMesh(), transform);
		}
	}

	for (auto& child : *entity->m_Children)
	{
		SubmitEntity(child, transform);
	}
}

void Renderer::SubmitMesh(Ref<MeshInstance> mesh, const glm::dmat4& transform)
{
	Material* material = mesh->GetMaterial();
	Shader* shader = material->GetShader();

	m_Meshes[shader].push_back({ mesh, material, transform });
}

void Renderer::SubmitLight()
{

}

//! TODO: Refactor and abstract
void Renderer::Process(double DeltaTime, Viewport* viewport)
{
	m_Meshes.clear();

	Entity* entity = viewport->m_Scene;
	while (entity)
	{
		if (!entity->m_Parent)
		{
			SubmitEntity(entity, glm::dmat4(1.0));
			break;
		}

		entity = entity->m_Parent;
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

	for (auto& [shader, commands] : m_Meshes)
	{
		shader->Bind();

		shader->Set("u_ProjectionMatrix", viewport->m_ProjectionMatrix);
		shader->Set("u_ViewMatrix", viewport->m_ViewMatrix);
		shader->Set("u_CameraPosition", viewport->m_CameraPosition);

		//shader->Set("u_NumPointLights", numPointLights);
		//shader->Set("u_NumDirectionalLights", numDirectionalLights);

		for (auto& command : commands)
		{
			command.m_Material->Bind(shader);

			shader->Set("u_ModelMatrix", command.m_Transform);

			command.m_Mesh->Render();

			command.m_Material->Unbind(shader);
		}

		shader->Unbind();
	}
}
