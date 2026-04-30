#pragma once
#include "positionBasedLight.h"

/** Spot Light Component
* Component for a spot light.
* The offset of the spot light is computed with the rotation and scale of the Entity.
*/
class SpotLightComponent : public PositionBasedLight
{
public:
	/** Apply the spot light to the lit shader. */
	void useLight(Shader& litShader, int lightIndex) override;

	/** Set the spot light use the diffuse color for the specular color (default specular color is white). */
	void setUseDiffColorToSpecColor(bool value);


	void setCutOffs(float newCutOff, float newOuterCutOff);

	void setDirection(Vector3 newDirection);
	void setCutOff(float newCutOff);
	void setOuterCutOff(float newOuterCutOff);

	Vector3 getDirection() const;
	float getCutOff() const;
	float getOuterCutOff() const;

protected:
	/** Called after the component has been created, but before it is registered. */
	void init() override;

	bool useColorToSpecular{ false };

	Vector3 direction{ Vector3::unitX };

	float cutOff{ 0.0f };
	float outerCutOff{ 0.0f };
};

