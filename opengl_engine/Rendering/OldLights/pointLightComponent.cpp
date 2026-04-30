#include "pointLightComponent.h"
#include <ServiceLocator/locator.h>

void PointLightComponent::useLight(Shader& litShader, int lightIndex)
{
	std::string light_index = std::to_string(lightIndex);

	litShader.setVec3("pointLights[" + light_index + "].position", position);

	litShader.setVec3("pointLights[" + light_index + "].ambient", lightColor.toVector() * ambientStrength);
	litShader.setVec3("pointLights[" + light_index + "].diffuse", lightColor.toVector() * diffuseStrength);
	Color spec_color = useColorToSpecular ? lightColor : Color::white;
	litShader.setVec3("pointLights[" + light_index + "].specular", spec_color.toVector());

	litShader.setFloat("pointLights[" + light_index + "].constant", constant);
	litShader.setFloat("pointLights[" + light_index + "].linear", linear);
	litShader.setFloat("pointLights[" + light_index + "].quadratic", quadratic);
}

void PointLightComponent::init()
{
	//  reset the values in case this component was used before (the component manager is a memory pool)
	resetValues();
	useColorToSpecular = false;


	lightType = LightType::EPointLight;

	initializePosition();
}

void PointLightComponent::setUseDiffColorToSpecColor(bool value)
{
	useColorToSpecular = value;
}
