#pragma once
#include "lightBase.h"
#include <ECS/component.h>
#include <Maths/Vector3.h>


/** Light Attenuation
* Defines the attenuation over distance of the light.
* See https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation to select values based on the range you want.
*/
struct LightAttenuation
{
	float constant;
	float linear;
	float quadratic;
};


/** Position Based Light
* Shared base values for specific light components that have a world position.
*/
class PositionBasedLight : public LightBase
{
public:
	/** The offset position from entity of the light. */
	Vector3 offset{ Vector3::zero };

	/** The attenuation of the light. Defaults to 50m. */
	LightAttenuation attenuation{ 1.0f, 0.09f, 0.032f };
};