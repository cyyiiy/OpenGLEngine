#pragma once
#include "vertexArray.h"


struct LoadMeshData
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};
	int matId{ 0 };
};


class Mesh
{
public:
	Mesh(LoadMeshData meshData);
	Mesh(const Mesh& other);
	Mesh& operator=(const Mesh&) = delete;
	~Mesh() {}

	int getMaterialIndex() const { return materialIndex; }

	void draw(bool drawAsLines = false);

private:
	VertexArray vertexArray;
	int materialIndex;
};