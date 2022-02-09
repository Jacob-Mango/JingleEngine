#include "Mesh.h"

#include <fstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Importer.hpp>

#include "Core/Core.h"

#include "Rendering/Material.h"

bool MeshAsset::OnLoad()
{
	if (GetPath() == "")
		return false;

	std::string file = GetPath();

	std::cout << "file: " << file << std::endl;

	uint32_t meshImportFlags = 0;
	meshImportFlags |= aiProcess_CalcTangentSpace;
	meshImportFlags |= aiProcess_Triangulate;
	meshImportFlags |= aiProcess_SortByPType;
	meshImportFlags |= aiProcess_GenNormals;
	meshImportFlags |= aiProcess_GenUVCoords;
	meshImportFlags |= aiProcess_OptimizeMeshes;
	meshImportFlags |= aiProcess_ValidateDataStructure;

	Assimp::Importer* importer = new Assimp::Importer();
	const aiScene* scene = importer->ReadFile(file, meshImportFlags);
	std::cout << "scene: " << PointerToString(scene) << std::endl;
	if (!scene || !scene->HasMeshes())
	{
		return false;
	}

	int vertexOffset = 0;
	int indexOffset = 0;

	for (size_t m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			glm::vec3 tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			glm::vec2 uv = { 0, 0 };

			if (mesh->HasTextureCoords(0))
				uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			m_Positions.push_back(pos);
			m_Normals.push_back(normal);
			m_Tangents.push_back(tangent);
			m_UV.push_back(uv);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			m_Indices.push_back(vertexOffset + mesh->mFaces[i].mIndices[0]);
			m_Indices.push_back(vertexOffset + mesh->mFaces[i].mIndices[1]);
			m_Indices.push_back(vertexOffset + mesh->mFaces[i].mIndices[2]);
		}

		vertexOffset += mesh->mNumVertices;
		indexOffset += mesh->mNumFaces * 3;
	}

	return true;
}

std::string MeshAsset::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

	ss << ", ";
	ss << "Positions=" << m_Positions.size();

	ss << ", ";
	ss << "Normals=" << m_Normals.size();

	ss << ", ";
	ss << "Tangents=" << m_Tangents.size();

	ss << ", ";
	ss << "UV=" << m_UV.size();

	ss << ", ";
	ss << "Indices=" << m_Indices.size();

	//ss << ", ";
	//ss << "Material=" << m_Material.AsString();

	return ss.str();
}

Mesh::Mesh(const Ref<MeshAsset>& asset) : m_Asset(asset), m_Material(m_Asset->m_Material)
{
	Create(asset->m_Positions, asset->m_Normals, asset->m_Tangents, asset->m_UV, asset->m_Indices);
}

Mesh::Mesh(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<unsigned int> indices) : m_Asset(nullptr), m_Material(material)
{
	Create(positions, std::vector<glm::vec3>(), std::vector<glm::vec3>(), std::vector<glm::vec2>(), indices);
}

Mesh::Mesh(const Ref<Material>& material, std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices) : m_Asset(nullptr), m_Material(material)
{
	Create(positions, std::vector<glm::vec3>(), std::vector<glm::vec3>(), uv, indices);
}

Mesh::Mesh(const Ref<Material>& material, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_Material(material)
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

Mesh::~Mesh()
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL(glDeleteBuffers(1, &m_VertexBufferObject));

	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GL(glDeleteBuffers(1, &m_IndexBufferObject));

	GL(glBindVertexArray(0));
	GL(glDeleteVertexArrays(1, &m_VertexArrayObject));
}

void Mesh::Create(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec2> uv, std::vector<unsigned int> indices)
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

Ref<Material> Mesh::GetMaterial()
{
	return m_Material;
}

void Mesh::SetMaterial(const Ref<Material>& material)
{
	m_Material = material;
}

void Mesh::Render()
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject));

	GL(glBindVertexArray(m_VertexArrayObject));

	GL(glDrawElements(GL_TRIANGLES, (GLsizei)m_IndexCount, GL_UNSIGNED_INT, 0));

	GL(glBindVertexArray(0));

	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

std::string Mesh::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

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
