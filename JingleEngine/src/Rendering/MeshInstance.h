#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#include "Rendering/Material.h"
#include "Rendering/Vertex.h"
#include "Rendering/Mesh.h"

#include <vector>

#include <glm/glm.hpp>

class MeshInstance : public Countable
{
	typedef Countable Super;

	friend class Entity;

private:
	GLuint m_VertexArrayObject = 0;
	GLuint m_VertexBufferObject = 0;
	GLuint m_IndexBufferObject = 0;

	unsigned int m_IndexCount = 0;

	Ref<Mesh> m_Asset;
	Ref<Material> m_Material;

public:
	MeshInstance(const Ref<Mesh>& asset);
	MeshInstance(const Ref<Material>& material, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	MeshInstance(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<unsigned int> indices);
	MeshInstance(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);
	virtual ~MeshInstance();

	void Create(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);

	Ref<Material> GetMaterial();
	void SetMaterial(const Ref<Material>& material);

	void Render();

	virtual std::string ToString() const override;
};
