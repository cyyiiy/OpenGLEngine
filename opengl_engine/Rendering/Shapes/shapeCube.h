#pragma once
#include "shapeBase.h"
#include <Maths/Geometry/box.h>


/** Shape Cube
* Drawable cube shape.
*/
class ShapeCube : public ShapeBase
{
public:
	ShapeCube(const Box& box, const Color& color);

	/**
	* Draw the cube shape. Don't call outside the rendering pipeline.
	* @param	shader		The shader to draw the shape with.
	*/
	void draw(const Shader& shader) const override;

	/** The box informations to draw the cube. */
	Box cubeBox;
};

