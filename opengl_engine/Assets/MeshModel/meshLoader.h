#pragma once
#include <Rendering/Model/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>


namespace MeshLoader
{
	static std::vector<LoadMeshData> LoadMeshes(const std::string& meshesPath);
	static void ProcessAssimpNode(aiNode* node, const aiScene* scene, std::vector<LoadMeshData>& meshesDatas);
	static LoadMeshData ProcessAssimpMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);
	static aiMatrix4x4 RetrieveAssimpParentTransform(aiNode* node);
};