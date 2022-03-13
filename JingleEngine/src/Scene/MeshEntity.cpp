#include "MeshEntity.h"

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

	if (!type.Model.IsNull())
	{
		m_Mesh = new Mesh(type.Model);

		if (!type.Material.IsNull())
		{
			m_Mesh->SetMaterial(type.Material);
		}
	}
}

Ref<Mesh> MeshEntity::GetMesh() const
{
	return m_Mesh;
}

std::string MeshEntity::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	ss << "Mesh=" << GetMesh().AsString();

	return ss.str();
}
