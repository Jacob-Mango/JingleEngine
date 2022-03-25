#include "MeshEntity.h"

#include "Scene/Components/MeshComponent.h"

BEGIN_CLASS_LINK(MeshEntityType)
END_CLASS_LINK()

BEGIN_CLASS_LINK(MeshEntity)
END_CLASS_LINK()

void MeshEntityType::Load(Config& config)
{
	Super::Load(config);

	{
		auto model = config["model"];
		auto path = model.String;

		Model = AssetModule::Get<MeshAsset>(path);
	}

	{
		auto model = config["material"];
		auto path = model.String;

		Material = AssetModule::Get<::Material>(path);
	}
}

void MeshEntity::OnCreate()
{
	auto& type = GetEntityType<MeshEntityType>();

	MeshComponent* component = AddComponent<MeshComponent>();

	if (!type.Model.IsNull())
	{
		Mesh* mesh = new Mesh(type.Model);
		component->SetMesh(mesh);

		if (!type.Material.IsNull())
		{
			mesh->SetMaterial(type.Material);
		}
	}
}

std::string MeshEntity::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	return ss.str();
}
