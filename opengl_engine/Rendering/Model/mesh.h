#pragma once
#include "vertexArray.h"
#include <vector>

using MaterialID = uint16_t;


struct MeshVerticesData
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};
	MaterialID matId{ 0 };
};


class Mesh
{
public:
	Mesh(MeshVerticesData meshData);

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	MaterialID getMaterialIndex() const { return materialIndex; }
	const VertexArray& getVertexArray() const { return vertexArray; }

private:
	VertexArray vertexArray;
	MaterialID materialIndex;
};