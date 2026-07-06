#include "mesh.h"

Mesh::Mesh(LoadMeshData meshData) : vertexArray(), materialIndex(meshData.matId)
{
	vertexArray.LoadVAMesh(meshData.vertices, meshData.indices);
}

Mesh::Mesh(const Mesh& other) : vertexArray(other.vertexArray), materialIndex(other.materialIndex)
{
}


void Mesh::draw(bool drawAsLines)
{
	// TODO: Put this in "RendererOpenGL" to have a mesh class as simple as possible?
	// Assume the shader is already in use (the rendering process should have done it)

	vertexArray.setActive();

	GLuint draw_method = drawAsLines ? GL_LINE_STRIP : GL_TRIANGLES;

	if (vertexArray.getUseEBO())
	{
		glDrawElements(draw_method, vertexArray.getNBIndices(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(draw_method, 0, vertexArray.getNBVertices());
	}
}