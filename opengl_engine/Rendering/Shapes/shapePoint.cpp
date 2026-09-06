#include "shapePoint.h"
#include <Maths/Matrix4.h>
#include <Rendering/Model/mesh.h>
#include <Assets/engineAssets.h>
#include <glad/glad.h>


ShapePoint::ShapePoint(const Vector3& position, const Color& color) : ShapeBase(color), pointPosition(position)
{}

void ShapePoint::draw(const Shader& shader) const
{
	// 1. Compute the point model matrix
	const Matrix4 point_model_matrix =
		Matrix4::createScale(POINT_MESH_SCALE) *
		Matrix4::createTranslation(pointPosition);

	// 2. Set the informations in the shader
	shader.setMatrix4("model", point_model_matrix.getAsFloatPtr());
	shader.setVec3("color", shapeColor);

	// 3. Draw the point vertex array (it's a cube)
	const VertexArray& cube_va = EngineAssets::GetVertexArray(EngineAssets::VertexArrayID::Cube);
	cube_va.setActive();

	if (cube_va.getUseEBO())
	{
		glDrawElements(GL_TRIANGLES, cube_va.getNBIndices(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, cube_va.getNBVertices());
	}
}
