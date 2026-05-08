#include "shapeCube.h"
#include <Maths/Matrix4.h>
#include <Rendering/Model/mesh.h>
#include <Assets/assetManager.h>


ShapeCube::ShapeCube(const Box& box, const Color& color) : ShapeBase(color), cubeBox(box)
{}

void ShapeCube::draw(Shader& shader) const
{
	// 1. Compute the box model matrix
	const Matrix4 box_model_matrix =
		Matrix4::createScale(cubeBox.getHalfExtents() * 2.0f) *
		Matrix4::createTranslation(cubeBox.getCenterPoint());

	// 2. Set the informations in the shader
	shader.setMatrix4("model", box_model_matrix.getAsFloatPtr());
	shader.setVec3("color", shapeColor);

	// 3. Draw the cube mesh
	Mesh& cube_mesh = AssetManager::GetSingleMesh("debug_cube");
	cube_mesh.draw(true);
}
