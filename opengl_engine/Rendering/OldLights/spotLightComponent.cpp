#include "spotLightComponent.h"
#include <ServiceLocator/locator.h>

void SpotLightComponent::useLight(Shader& litShader, int lightIndex)
{
	std::string light_index = std::to_string(lightIndex);

	litShader.setVec3("spotLights[" + light_index + "].position", position);
	litShader.setVec3("spotLights[" + light_index + "].direction", direction);

	litShader.setVec3("spotLights[" + light_index + "].ambient", lightColor.toVector() * ambientStrength);
	litShader.setVec3("spotLights[" + light_index + "].diffuse", lightColor.toVector() * diffuseStrength);
	Color spec_color = useColorToSpecular ? lightColor : Color::white;
	litShader.setVec3("spotLights[" + light_index + "].specular", spec_color.toVector());

	litShader.setFloat("spotLights[" + light_index + "].cutOff", cutOff);
	litShader.setFloat("spotLights[" + light_index + "].outerCutOff", outerCutOff);

	litShader.setFloat("spotLights[" + light_index + "].constant", constant);
	litShader.setFloat("spotLights[" + light_index + "].linear", linear);
	litShader.setFloat("spotLights[" + light_index + "].quadratic", quadratic);
}

void SpotLightComponent::init()
{
	//  reset the values in case this component was used before (the component manager is a memory pool)
	resetValues();
	useColorToSpecular = false;
	direction = Vector3::unitX;
	cutOff = 0.0f;
	outerCutOff = 0.0f;


	lightType = LightType::ESpotLight;

	initializePosition();
}

void SpotLightComponent::setUseDiffColorToSpecColor(bool value)
{
	useColorToSpecular = value;
}


void SpotLightComponent::setCutOffs(float newCutOff, float newOuterCutOff)
{
	cutOff = newCutOff;
	outerCutOff = newOuterCutOff;
}

void SpotLightComponent::setDirection(Vector3 newDirection)
{
	direction = newDirection;
}

void SpotLightComponent::setCutOff(float newCutOff)
{
	cutOff = newCutOff;
}

void SpotLightComponent::setOuterCutOff(float newOuterCutOff)
{
	outerCutOff = newOuterCutOff;
}

Vector3 SpotLightComponent::getDirection() const
{
	return direction;
}

float SpotLightComponent::getCutOff() const
{
	return cutOff;
}

float SpotLightComponent::getOuterCutOff() const
{
	return outerCutOff;
}
