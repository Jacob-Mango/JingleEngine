#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#include <JingleScript.h>

#include "Rendering/Material.h"

typedef struct
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec2 UV;
} Vertex;

class Mesh;

class MeshAsset : public Asset
{
	typedef Asset Super;

	friend class Mesh;

	SETUP_ASSET(MeshAsset, Asset)

private:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec3> m_Tangents;
	std::vector<glm::vec2> m_UV;
	std::vector<unsigned int> m_Indices;

	Ref<Material> m_Material;

public:
	virtual bool OnLoad() override;

	virtual std::string ToString() const override;
};

class Mesh : public Countable
{
	typedef Countable Super;

	friend class Entity;

private:
	GLuint m_VertexArrayObject = 0;
	GLuint m_VertexBufferObject = 0;
	GLuint m_IndexBufferObject = 0;

	unsigned int m_IndexCount = 0;

	Ref<MeshAsset> m_Asset;
	Ref<Material> m_Material;

public:
	Mesh(const Ref<MeshAsset>& asset);
	Mesh(const Ref<Material>& material, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	Mesh(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<unsigned int> indices);
	Mesh(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);
	virtual ~Mesh();

	void Create(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);

	Ref<Material> GetMaterial();
	void SetMaterial(const Ref<Material>& material);

	void Render();

	virtual std::string ToString() const override;
};
