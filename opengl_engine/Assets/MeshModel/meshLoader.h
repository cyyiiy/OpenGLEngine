#pragma once
#include <Rendering/Model/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>


namespace MeshLoader
{
	std::vector<LoadMeshData> LoadMeshes(const std::string& meshesPath);
	void ProcessAssimpNode(aiNode* node, const aiScene* scene, std::vector<LoadMeshData>& meshesDatas);
	LoadMeshData ProcessAssimpMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);
	aiMatrix4x4 RetrieveAssimpParentTransform(aiNode* node);
};