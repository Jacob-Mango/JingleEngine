#include "Scene/Components/MeshComponent.h"

#include "Rendering/Renderer.h"

void MeshComponent::Submit(const glm::mat4& InTransform, Renderer* Renderer)
{
	Renderer->SubmitStaticMesh(GetMesh(), InTransform * GetTransform());
}

BEGIN_CLASS_LINK(MeshComponent)
END_CLASS_LINK()
