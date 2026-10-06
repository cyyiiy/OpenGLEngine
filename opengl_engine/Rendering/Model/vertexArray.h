#pragma once
#include <Maths/vector2.h>
#include <Maths/vector3.h>
#include <vector>


struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoords;
};


class VertexArray
{
public:
	// ----- Constructors -----
	VertexArray() {}
	~VertexArray() { releaseVertexArray(); }

	// Remove copy constructors
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	// Define move constructors
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;


	// ----- Vertex Array Loaders (could be moved elsewhere in theory) -----
	void LoadVAMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void LoadVAQuadHUD();
	void LoadVALine();
	void LoadVABillboard();


	// ----- Vertex Array usage functions -----
	void setActive() const;

	unsigned int getNBVertices() const { return nbVertices; }
	unsigned int getNBIndices() const { return nbIndices; }

	unsigned int getVAO() const { return VAO; }
	unsigned int getVBO() const { return VBO; }
	unsigned int getEBO() const { return EBO; }

	bool getUseEBO() const { return useEBO; }


	uint64_t getVertexArrayGPUMemory() const;


private:
	unsigned int nbVertices{ 0 };
	unsigned int nbIndices{ 0 };

	bool useEBO{ false };

	unsigned int VAO{ 0 }; // OpenGL ID
	unsigned int VBO{ 0 }; // OpenGL ID
	unsigned int EBO{ 0 }; // OpenGL ID

	void releaseVertexArray();
};
