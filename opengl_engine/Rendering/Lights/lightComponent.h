#pragma once
#include <ECS/component.h>
#include <Utils/color.h>


/** Light Component
* Abstract base class for specific light components.
*/
class LightComponent : public Component
{
public:
	/** Wether or not the light is used by the renderer. */
	bool active{ true };

	/** The tint color of the light. */
	Color lightColor{ Color::white };

	/** The ambient strength of the light. Phong Lighting specific. */
	float ambientStrength{ 0.01f };

	/** The diffuse strength of the light. Phong Lighting specific. */
	float diffuseStrength{ 0.7f };
};