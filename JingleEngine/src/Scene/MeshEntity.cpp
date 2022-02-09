#include "MeshEntity.h"

void MeshEntityType::Load(Config& config)
{
	super::Load(config);

	{
		auto model = config["model"];
		auto path = model.String;

		Model = AssetManager::Get<MeshAsset>(path);
	}

	{
		auto model = config["material"];
		auto path = model.String;

		Material = AssetManager::Get<::Material>(path);
	}
}

void MeshEntity::OnCreate()
{
	if (!GetType()->Model.IsNull())
	{
		m_Mesh = new Mesh(GetType()->Model);

		if (!GetType()->Material.IsNull())
		{
			m_Mesh->SetMaterial(GetType()->Material);
		}
	}
}

Ref<Mesh> MeshEntity::GetMesh()
{
	return m_Mesh;
}

std::string MeshEntity::ToString()
{
	std::stringstream ss;

	ss << Entity::ToString();

	ss << ", ";
	ss << "Mesh=" << m_Mesh.AsString();

	return ss.str();
}
