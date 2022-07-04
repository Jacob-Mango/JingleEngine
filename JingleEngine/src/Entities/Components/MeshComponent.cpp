#include "Entities/Components/MeshComponent.h"

#include "Rendering/Renderer.h"

BEGIN_CLASS_LINK(MeshComponent)
	LINK_VARIABLE(Model);
	LINK_VARIABLE(Material);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(Entity*);
END_CLASS_LINK()

void MeshComponent::OnCreate()
{
	m_Mesh = new MeshInstance(Model);

	if (Material != nullptr)
	{
		m_Mesh->SetMaterial(Material);
	}
}

void MeshComponent::Editor_OnPropertyChanged(std::string name)
{
	if (name.compare("Model") == 0)
	{
		delete m_Mesh;

		OnCreate();

		return;
	}

	if (name.compare("Material") == 0)
	{
		if (m_Mesh)
		{
			m_Mesh->SetMaterial(Material);
		}

		return;
	}

	Super::Editor_OnPropertyChanged(name);
}
