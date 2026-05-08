#include "shapePoint.h"
#include <Maths/Matrix4.h>
#include <Rendering/Model/mesh.h>
#include <Assets/assetManager.h>


ShapePoint::ShapePoint(const Vector3& position, const Color& color) : ShapeBase(color), pointPosition(position)
{}

void ShapePoint::draw(Shader& shader) const
{
	// 1. Compute the point model matrix
	const Matrix4 point_model_matrix =
		Matrix4::createScale(POINT_MESH_SCALE) *
		Matrix4::createTranslation(pointPosition);

	// 2. Set the informations in the shader
	shader.setMatrix4("model", point_model_matrix.getAsFloatPtr());
	shader.setVec3("color", shapeColor);

	// 3. Draw the point mesh (it's a cube)
	Mesh& point_mesh = AssetManager::GetSingleMesh("debug_cube");
	point_mesh.draw(false);
}
