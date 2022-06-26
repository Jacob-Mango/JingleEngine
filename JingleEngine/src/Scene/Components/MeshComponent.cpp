#include "Scene/Components/MeshComponent.h"

#include "Rendering/Renderer.h"

BEGIN_CLASS_LINK(MeshComponent)
	LINK_VARIABLE(Model);
	LINK_VARIABLE(MaterialOverride);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(Entity*);
END_CLASS_LINK()

void MeshComponent::OnCreate()
{
	m_Mesh = new MeshInstance(Model);

	if (MaterialOverride != nullptr)
	{
		m_Mesh->SetMaterial(MaterialOverride);
	}
}
