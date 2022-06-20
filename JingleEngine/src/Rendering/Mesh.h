#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#include "Rendering/Material.h"
#include "Rendering/Vertex.h"

#include <vector>

#include <glm/glm.hpp>

class Mesh : public Asset
{
	friend class MeshInstance;

	SETUP_ASSET(Mesh, Asset)

private:
	//TODO: submeshes
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	Ref<Material> m_Material;

public:
	Mesh();
	virtual ~Mesh();

	virtual bool OnLoad() override;

	virtual std::string ToString() const override;
};
