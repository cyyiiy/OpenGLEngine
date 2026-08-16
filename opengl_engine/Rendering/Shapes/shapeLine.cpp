#include "shapeLine.h"
#include <Maths/Matrix4.h>
#include <Rendering/Model/vertexArray.h>
#include <Assets/engineAssets.h>


ShapeLine::ShapeLine(const Vector3& pointA, const Vector3& pointB, const Color& color) : ShapeBase(color), linePointA(pointA), linePointB(pointB)
{}

void ShapeLine::draw(Shader& shader) const
{
	// 1. Compute the origin (point A) model matrix
	const Matrix4 origin_model_matrix = Matrix4::createTranslation(linePointA);

	// 2. Set the informations in the shader
	shader.setMatrix4("model", origin_model_matrix.getAsFloatPtr());
	shader.setVec3("color", shapeColor);
	shader.setBool("renderLine", true);
	shader.setVec3("linePointOffset", linePointB - linePointA);

	// 3. Draw the line vertex array
	const VertexArray& line_va = EngineAssets::GetVertexArray(EngineAssets::VertexArrayID::Line);
	line_va.setActive();
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	// 4. Disable the 'renderLine' flag on the shader
	shader.setBool("renderLine", false);
}
