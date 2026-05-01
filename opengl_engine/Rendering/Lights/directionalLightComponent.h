#pragma once
#include "lightComponent.h"
#include <Maths/Vector3.h>


/** Directional Light Component
* A data-only component for a directional light.
*/
class DirectionalLightComponent : public LightComponent
{
public:
	/** The direction of the directional light. */
	Vector3 direction{ Vector3::unitX };
};