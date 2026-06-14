#pragma once
#include <ECS/component.h>
#include <Rendering/Shapes/shapeBase.h>
#include <memory>


/** Shape Renderer Component
* A data-only component that holds informations to render a drawable shape for debug purposes.
* The shape rendering does not uses the entity transform it is attached to.
*/
class ShapeRendererComponent : public Component
{
public:
	/** The shape to draw. */
	std::shared_ptr<ShapeBase> shape;

	/** Duration (in seconds) before this shape expires. */
	float lifetime{ 0.0f };
};