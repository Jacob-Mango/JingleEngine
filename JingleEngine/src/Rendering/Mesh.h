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
	typedef Asset Super;

	friend class MeshInstance;

	SETUP_ASSET(Mesh, Asset)

private:
	//TODO: submeshes
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec3> m_Tangents;
	std::vector<glm::vec2> m_UV;
	std::vector<unsigned int> m_Indices;

	Ref<Material> m_Material;

public:
	Mesh();
	virtual ~Mesh();

	virtual bool OnLoad() override;

	virtual std::string ToString() const override;
};
