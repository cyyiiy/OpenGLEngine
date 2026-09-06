#include "shapeCube.h"
#include <Maths/Matrix4.h>
#include <Rendering/Model/mesh.h>
#include <Assets/engineAssets.h>
#include <glad/glad.h>


ShapeCube::ShapeCube(const Box& box, const Color& color) : ShapeBase(color), cubeBox(box)
{}

void ShapeCube::draw(const Shader& shader) const
{
	// 1. Compute the box model matrix
	const Matrix4 box_model_matrix =
		Matrix4::createScale(cubeBox.getHalfExtents() * 2.0f) *
		Matrix4::createTranslation(cubeBox.getCenterPoint());

	// 2. Set the informations in the shader
	shader.setMatrix4("model", box_model_matrix.getAsFloatPtr());
	shader.setVec3("color", shapeColor);

	// 3. Draw the cube vertex array
	const VertexArray& cube_va = EngineAssets::GetVertexArray(EngineAssets::VertexArrayID::Cube);
	cube_va.setActive();

	if (cube_va.getUseEBO())
	{
		glDrawElements(GL_LINE_STRIP, cube_va.getNBIndices(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_LINE_STRIP, 0, cube_va.getNBVertices());
	}
}
