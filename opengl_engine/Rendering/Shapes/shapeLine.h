#pragma once
#include "shapeBase.h"


/** Shape Line
* Drawable line shape.
*/
class ShapeLine : public ShapeBase
{
public:
	ShapeLine(const Vector3& pointA, const Vector3& pointB, const Color& color);

	/**
	* Draw the line shape. Don't call outside the rendering pipeline.
	* @param	shader		The shader to draw the shape with.
	*/
	void draw(Shader& shader) const override;

	Vector3 linePointA;

	Vector3 linePointB;
};

