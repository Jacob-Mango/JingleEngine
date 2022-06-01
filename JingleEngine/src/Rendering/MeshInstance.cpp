#include "Rendering/MeshInstance.h"

MeshInstance::MeshInstance(const Ref<Mesh>& asset) : m_Asset(asset), m_Material(m_Asset->m_Material)
{
	Create(asset->m_Positions, asset->m_Normals, asset->m_Tangents, asset->m_UV, asset->m_Indices);
}

MeshInstance::MeshInstance(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<unsigned int> indices) : m_Asset(nullptr), m_Material(material)
{
	Create(positions, std::vector<glm::vec3>(), std::vector<glm::vec3>(), std::vector<glm::vec2>(), indices);
}

MeshInstance::MeshInstance(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices) : m_Asset(nullptr), m_Material(material)
{
	Create(positions, std::vector<glm::vec3>(), std::vector<glm::vec3>(), uv, indices);
}

MeshInstance::MeshInstance(const Ref<Material>& material, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_Material(material)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uv;

	for (int i = 0; i < vertices.size(); i++)
	{
		positions.push_back(vertices[i].Position);
		normals.push_back(vertices[i].Normal);
		tangents.push_back(vertices[i].Tangent);
		uv.push_back(vertices[i].UV);
	}

	Create(positions, normals, tangents, uv, indices);
}

MeshInstance::~MeshInstance()
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL(glDeleteBuffers(1, &m_VertexBufferObject));

	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GL(glDeleteBuffers(1, &m_IndexBufferObject));

	GL(glBindVertexArray(0));
	GL(glDeleteVertexArrays(1, &m_VertexArrayObject));
}

void MeshInstance::Create(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec2> uv, std::vector<unsigned int> indices)
{
	GL(glGenVertexArrays(1, &m_VertexArrayObject));
	GL(glBindVertexArray(m_VertexArrayObject));

	int offset = 0;
	int size = 0;
	int index = 0;

	bool hasNormals = normals.size() > 0;
	bool hasUVs = uv.size() > 0;

	{
		size += SIZE_OF(positions, glm::vec3);
	}

	if (hasNormals)
	{
		size += SIZE_OF(normals, glm::vec3);
	}

	if (hasNormals)
	{
		size += SIZE_OF(tangents, glm::vec3);
	}

	if (hasUVs)
	{
		size += SIZE_OF(uv, glm::vec2);
	}

	GL(glGenBuffers(1, &m_VertexBufferObject));
	GL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject));
	GL(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW));

	GL(glGenBuffers(1, &m_IndexBufferObject));
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject));
	GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_OF(indices, unsigned int), indices.data(), GL_STATIC_DRAW));

	offset = 0;
	size = 0;

	{
		size = SIZE_OF(positions, glm::vec3);
		GL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, positions.data()));
		offset += size;
	}

	if (hasNormals)
	{
		size = SIZE_OF(normals, glm::vec3);
		GL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, normals.data()));
		offset += size;
	}

	if (hasNormals)
	{
		size = SIZE_OF(tangents, glm::vec3);
		GL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, tangents.data()));
		offset += size;
	}

	if (hasUVs)
	{
		size = SIZE_OF(uv, glm::vec2);
		GL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, uv.data()));
		offset += size;
	}

	offset = 0;
	size = 0;

	{
		size = sizeof(glm::vec3);
		GL(glEnableVertexAttribArray(index));
		GL(glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, size, (void*)offset));
		offset += SIZE_OF(positions, glm::vec3);
		index++;
	}

	if (hasNormals)
	{
		size = sizeof(glm::vec3);
		GL(glEnableVertexAttribArray(index));
		GL(glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, size, (void*)offset));
		offset += SIZE_OF(normals, glm::vec3);
		index++;
	}

	if (hasNormals)
	{
		size = sizeof(glm::vec3);
		GL(glEnableVertexAttribArray(index));
		GL(glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, size, (void*)offset));
		offset += SIZE_OF(tangents, glm::vec3);
		index++;
	}

	if (hasUVs)
	{
		size = sizeof(glm::vec2);
		GL(glEnableVertexAttribArray(index));
		GL(glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, size, (void*)offset));
		offset += SIZE_OF(uv, glm::vec2);
		index++;
	}

	GL(glBindVertexArray(0));
	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	m_IndexCount = indices.size();
}

Ref<Material> MeshInstance::GetMaterial()
{
	return m_Material;
}

void MeshInstance::SetMaterial(const Ref<Material>& material)
{
	m_Material = material;
}

void MeshInstance::Render()
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject));

	GL(glBindVertexArray(m_VertexArrayObject));

	GL(glDrawElements(GL_TRIANGLES, (GLsizei)m_IndexCount, GL_UNSIGNED_INT, 0));

	GL(glBindVertexArray(0));

	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

std::string MeshInstance::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	ss << "Asset=" << m_Asset.AsString();

	ss << ", ";
	ss << "Material=" << m_Material.AsString();

	ss << ", ";
	ss << "VertexArrayObject=" << m_VertexArrayObject;

	ss << ", ";
	ss << "VertexBufferObject=" << m_VertexBufferObject;

	ss << ", ";
	ss << "IndexBufferObject=" << m_IndexBufferObject;

	return ss.str();
}
