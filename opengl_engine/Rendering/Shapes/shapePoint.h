#pragma once
#include "shapeBase.h"
#include <Maths/Vector3.h>

const float POINT_MESH_SCALE = 0.05f;


/** Shape Point
* Drawable point shape.
*/
class ShapePoint : public ShapeBase
{
public:
	ShapePoint(const Vector3& position, const Color& color);

	/**
	* Draw the point shape. Don't call outsiude the rendering pipeline.
	* @param	shader		The shader to draw the shape with.
	*/
	void draw(const Shader& shader) const override;

	/** The position of the point to draw. */
	Vector3 pointPosition;
};

