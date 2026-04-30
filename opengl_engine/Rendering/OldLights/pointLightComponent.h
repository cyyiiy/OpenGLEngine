#pragma once
#include "positionBasedLight.h"

/** Point Light Component
* Component for a point light.
* The offset of the point light is computed with the rotation and scale of the Entity.
*/
class PointLightComponent : public PositionBasedLight
{
public:
	/** Apply the point light to the lit shader. */
	void useLight(Shader& litShader, int lightIndex) override;

	/** Set the point light use the diffuse color for the specular color (default specular color is white). */
	void setUseDiffColorToSpecColor(bool value);

protected:
	/** Called after the component has been created, but before it is registered. */
	void init() override;

	bool useColorToSpecular{ false };
};

