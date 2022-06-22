#include "Scene/Components/MeshComponent.h"

#include "Rendering/Renderer.h"

BEGIN_CLASS_LINK(MeshComponent)
	LINK_VARIABLE(Model);
	LINK_VARIABLE(Material);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(Entity*);
END_CLASS_LINK()

void MeshComponent::OnCreate()
{
	m_Mesh = new MeshInstance(AssetModule::Get<Mesh>(Model));

	auto material = AssetModule::Get<::Material>(Material);
	if (material != nullptr)
	{
		m_Mesh->SetMaterial(material);
	}
}
