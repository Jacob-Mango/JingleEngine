#include "Rendering/MeshInstance.h"

MeshInstance::MeshInstance(const Ref<Mesh>& asset) : m_Asset(asset), m_Material(m_Asset->m_Material)
{
	Create(asset->m_Vertices, asset->m_Indices);
}

MeshInstance::MeshInstance(const Ref<Material>& material, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_Material(material)
{
	Create(vertices, indices);
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

void MeshInstance::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	GL(glGenVertexArrays(1, &m_VertexArrayObject));
	GL(glBindVertexArray(m_VertexArrayObject));

	GL(glGenBuffers(1, &m_VertexBufferObject));
	GL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject));
	GL(glBufferData(GL_ARRAY_BUFFER, SIZE_OF(vertices, Vertex), vertices.data(), GL_STATIC_DRAW));

	GL(glGenBuffers(1, &m_IndexBufferObject));
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject));
	GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_OF(indices, unsigned int), indices.data(), GL_STATIC_DRAW));

	const int POSITION_INDEX = 0;
	const int NORMAL_INDEX = 1;
	const int TANGENT_INDEX = 2;
	const int UV_INDEX = 3;

	GL(glEnableVertexAttribArray(POSITION_INDEX));
	GL(glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position)));

	GL(glEnableVertexAttribArray(NORMAL_INDEX));
	GL(glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	GL(glEnableVertexAttribArray(TANGENT_INDEX));
	GL(glVertexAttribPointer(TANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

	GL(glEnableVertexAttribArray(UV_INDEX));
	GL(glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV)));

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
