#include "mesh.h"

Mesh::Mesh(LoadMeshData meshData) : vertexArray(), materialIndex(meshData.matId)
{
	vertexArray.LoadVAMesh(meshData.vertices, meshData.indices);
}

Mesh::Mesh(const Mesh& other) : vertexArray(other.vertexArray), materialIndex(other.materialIndex)
{
}