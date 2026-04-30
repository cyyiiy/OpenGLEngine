#pragma once
#include "lightComponent.h"
#include <Maths/Vector3.h>

/** Directional Light Component
* Component for a directional light.
* This component doesn't use the transform of the Entity.
*/
class DirectionalLightComponent : public LightComponent
{
public:
	/** Apply the directional light to the lit shader. */
	void useLight(Shader& litShader, int lightIndex) override;


	void setDirection(Vector3 newDirection);

	Vector3 getDirection() const;

protected:
	/** Called after the component has been created, but before it is registered. */
	void init() override;

	Vector3 direction{ Vector3::unitX };
};

