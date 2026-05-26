#pragma once
#include <Utils/Color.h>
#include <Rendering/shader.h>


/** Shape Base
* Base class for all drawable shapes.
*/
class ShapeBase
{
public:
	ShapeBase(const Color& color) : shapeColor(color) {}
	virtual ~ShapeBase() {}

	/**
	* Draw the shape. Don't call outside the rendering pipeline.
	* @param	shader		The shader to draw the shape with.
	*/
	virtual void draw(Shader& shader) const = 0;

	/** The color to draw the shape with. */
	Color shapeColor;
};