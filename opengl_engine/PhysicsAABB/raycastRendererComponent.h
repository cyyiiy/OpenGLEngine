#pragma once
#include <ECS/component.h>
#include <Rendering/Shapes/shapeBase.h>
#include <vector>
#include <memory>


/** Raycast Renderer Component
* A data-only component that holds informations to render a raycast for debug purposes.
* The raycast rendering does not uses the entity transfom it is attached to.
*/
class RaycastRendererComponent : public Component
{
public:
	/** The shapes making the raycast to draw. */
	std::vector<std::shared_ptr<ShapeBase>> shapes;

	/** Duration (in seconds) before this raycast expires. */
	float lifetime{ 0.0f };

	/** True if the raycast must be cleared on scene exit. */
	bool registeredOnScene{ false };
};


// Specify sublist size for 'RaycastRendererComponent'
template<>
struct ComponentSublistSize<RaycastRendererComponent>
{
	static constexpr size_t value = 16;
};