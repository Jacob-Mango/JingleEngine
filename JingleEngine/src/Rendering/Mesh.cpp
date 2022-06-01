#include "Rendering/Mesh.h"

#include <fstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Importer.hpp>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{

}

BEGIN_CLASS_LINK(Mesh)
END_CLASS_LINK()

bool Mesh::OnLoad()
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

	JS_INFO("Mesh '%s'", ToString().c_str());

	return true;
}

std::string Mesh::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

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
