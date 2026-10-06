#pragma once
#include <ECS/component.h>
#include <Rendering/texture.h>
#include <Utils/Color.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>
#include <memory>


/** Billboard Renderer Component
* A data-only component that holds informations to render a billboard.
*/
class BillboardRendererComponent : public Component
{
public:
	/** The billboard texture to render. */
	std::shared_ptr<Texture> billboardTexture{ nullptr };

	/** The scale of the billboard on scene. Default is a one scene-unit square. */
	Vector2 billboardScale{ Vector2::one };

	/** Tint color to apply to the billboard texture. */
	Color tintColor{ Color::white };

	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The position offset of this Billboard Renderer Component. */
	Vector3 positionOffset{ Vector3::zero };
};


// Specify sublist size for 'BillboardRendererComponent'
template<>
struct ComponentSublistSize<BillboardRendererComponent>
{
	static constexpr size_t value = 16;
};