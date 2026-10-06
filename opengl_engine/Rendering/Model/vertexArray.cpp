#include "vertexArray.h"
#include <glad/glad.h>

void VertexArray::LoadVAMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	nbVertices = static_cast<unsigned int>(vertices.size());
	nbIndices = static_cast<unsigned int>(indices.size());
	useEBO = indices.size() > 0;

	if (vertices.size() == 0) return;

	// Setup vertex buffer object and vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO); // Bind the VAO before binding the vertex buffer, and before configuring vertex attributes 

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nbVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (useEBO) // Setup EBO if specified
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Texture coordinates attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	// Unbind vertex array
	glBindVertexArray(0);
}

void VertexArray::LoadVAQuadHUD()
{
	// Create vertices array for quad
	GLfloat quad_vertices[] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	// Setup vertex buffer object and vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO); // Bind vertex array

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

	// Vertex attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex array and vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexArray::LoadVALine()
{
	// Create vertices array for line
	GLfloat line_vertices[] =
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f
	};

	// Setup vertex buffer object and vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO); // Bind vertex array

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);

	// Vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex array and vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexArray::LoadVABillboard()
{
	// Create point vertex
	GLfloat billboard_point[] =
	{ 
		0.0f, 0.0f, 0.0f 
	};

	// Setup vertex buffer object and vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO); // Bind vertex array

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(billboard_point), billboard_point, GL_STATIC_DRAW);

	// Vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex array and vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void VertexArray::setActive() const
{
	glBindVertexArray(VAO);
}

uint64_t VertexArray::getVertexArrayGPUMemory() const
{
	return nbVertices * sizeof(Vertex) + nbIndices * sizeof(unsigned int);
}


VertexArray::VertexArray(VertexArray&& other) noexcept :
	nbVertices(other.nbVertices), nbIndices(other.nbIndices),
	useEBO(other.useEBO), VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
{
	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		releaseVertexArray();

		nbVertices = other.nbVertices;
		nbIndices = other.nbIndices;
		useEBO = other.useEBO;
		VAO = other.VAO;
		VBO = other.VBO;
		EBO = other.EBO;

		other.VAO = 0;
		other.VBO = 0;
		other.EBO = 0;
	}

	return *this;
}


void VertexArray::releaseVertexArray()
{
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
}