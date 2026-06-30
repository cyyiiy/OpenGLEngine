#pragma once
#include <ECS/component.h>
#include <Rendering/texture.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>


/** Billboard Renderer Component
* A data-only component that holds informations to render a billboard.
*/
class BillboardRendererComponent : public Component
{
public:
	/** The billboard texture to render. */
	Texture* billboardTexture{ nullptr };

	/** The scale of the billboard on scene. Default is a one scene-unit square. */
	Vector2 billboardScale{ Vector2::one };

	/** True if the model need to ignore its owner transform. Offset will then be computed from world origin (0;0;0) */
	bool ignoreOwnerTransform{ false };

	/** The position offset of this Billboard Renderer Component. */
	Vector3 positionOffset{ Vector3::zero };
};