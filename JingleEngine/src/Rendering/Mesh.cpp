#include "Rendering/Mesh.h"

#include <fstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Importer.hpp>

Mesh::Mesh()
{
	m_Material = nullptr;
}

Mesh::~Mesh()
{

}

BEGIN_CLASS_LINK(Mesh)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

bool Mesh::OnLoad()
{
	JS_TRACE(Tracers::Rendering);

	std::string file = GetPath();

	JS_TINFO("File '{}'", file);

	uint32_t meshImportFlags = 0;
	meshImportFlags |= aiProcessPreset_TargetRealtime_MaxQuality;

	Assimp::Importer* importer = new Assimp::Importer();
	const aiScene* scene = importer->ReadFile(file, meshImportFlags);
	JS_TINFO("aiScene '{}'", PointerToString(scene));
	if (!scene || !scene->HasMeshes())
	{
		return false;
	}

	int vertexOffset = 0;
	int indexOffset = 0;

	int numMeshes = scene->mNumMeshes > 0 ? 1 : 0;

	for (size_t m = 0; m < numMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];

		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
			vertex.UV = { 0, 0 };

			if (mesh->HasTextureCoords(0))
				vertex.UV = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			m_Vertices.push_back(vertex);
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

	JS_TINFO("Mesh '{}'", ToString());

	return true;
}

std::string Mesh::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	ss << "Vertices=" << m_Vertices.size();

	ss << ", ";
	ss << "Indices=" << m_Indices.size();

	ss << ", ";
	ss << "Material=" << m_Material.AsString();

	return ss.str();
}
