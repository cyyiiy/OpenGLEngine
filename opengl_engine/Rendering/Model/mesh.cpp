#include "mesh.h"

Mesh::Mesh(MeshVerticesData meshData) : vertexArray(), materialIndex(meshData.matId)
{
	vertexArray.LoadVAMesh(meshData.vertices, meshData.indices);
}