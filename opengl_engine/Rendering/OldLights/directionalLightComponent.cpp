#include "directionalLightComponent.h"
#include <ServiceLocator/locator.h>

void DirectionalLightComponent::useLight(Shader& litShader, int lightIndex)
{
	litShader.setVec3("dirLight.direction", direction);

	litShader.setVec3("dirLight.ambient", lightColor.toVector() * ambientStrength);
	litShader.setVec3("dirLight.diffuse", lightColor.toVector() * diffuseStrength);
	Color spec_color = Color::white;
	litShader.setVec3("dirLight.specular", spec_color.toVector());
}

void DirectionalLightComponent::init()
{
	//  reset the values in case this component was used before (the component manager is a memory pool)
	active = true;
	lightColor = Color::white;
	ambientStrength = 0.01f;
	diffuseStrength = 0.7f;
	direction = Vector3::unitX;


	lightType = LightType::EDirectionalLight;
}

void DirectionalLightComponent::setDirection(Vector3 newDirection)
{
	direction = newDirection;
}

Vector3 DirectionalLightComponent::getDirection() const
{
	return direction;
}
