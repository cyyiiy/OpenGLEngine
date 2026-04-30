#pragma once
#include <ECS/component.h>
#include <Rendering/shader.h>
#include <Utils/color.h>


enum LightType : uint8_t
{
	EUndefinedLight = 0,
	EDirectionalLight = 1,
	EPointLight = 2,
	ESpotLight = 3
};

/** Light Component
* Base class for specific light components.
* Abstract Component - do not add it to an Entity.
*/
class LightComponent : public Component
{
public:
	virtual ~LightComponent() {}


	/**
	* Apply the light to the lit shader.
	* This function must be overriden by every light type that inherit this component.
	* This function is called by the rendering sequence, do not call it manually.
	* @param	litShader		The lit shader to use this light on.
	* @param	lightIndex		The index of this light (by type).
	*/
	virtual void useLight(Shader& litShader, int lightIndex) = 0;


	/** Get the type of this light. */
	LightType getLightType();

	/** Set the active value of this light to turn it on or off. */
	void setActive(bool activeValue);

	/** Know if the light is active or not. */
	bool isActive();


	void setColor(Color newColor);
	void setAmbientStrength(float newAmbientStrength);
	void setDiffuseStrength(float newDiffuseStrength);

	Color getColor();
	float getAmbientStrength();
	float getDiffuseStrength();


protected:
	virtual void registerComponent() override;
	virtual void unregisterComponent() override;

	LightType lightType{ EUndefinedLight };

	bool active{ true };

	Color lightColor{ Color::white };
	float ambientStrength{ 0.01f };
	float diffuseStrength{ 0.7f };
};