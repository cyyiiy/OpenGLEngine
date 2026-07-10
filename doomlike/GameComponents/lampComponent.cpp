#include "lampComponent.h"
#include <ECS/entity.h>
#include <Assets/assetManager.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/modelRendererComponent.h>

#include <stdlib.h>
#include <time.h>


void LampComponent::setup(ComponentHandle<PointLightComponent> lightComp, ComponentHandle<ModelRendererComponent> modelRendererComp, bool setupChandelier)
{
	light = lightComp;
	modelRenderer = modelRendererComp;

	isChandelier = setupChandelier;
	baseLightIntensity = isChandelier ? 0.4f : 0.22f;

	srand(static_cast<unsigned int>(time(NULL)));
	timer = float(rand() % 100) / 100.0f;

	ECS::GetComponent(light).diffuseStrength = baseLightIntensity + (Maths::sin(timer) / 11.0f);

	compValid = ECS::IsComponentHandleValid(light) && ECS::IsComponentHandleValid(modelRenderer); // Prevent wrongly initialized lamp component to crash the game
}

void LampComponent::changeStatus(bool lightOn)
{
	if (!compValid) return;

	PointLightComponent& light_comp = ECS::GetComponent(light);
	ModelRendererComponent& model_comp = ECS::GetComponent(modelRenderer);

	light_comp.active = lightOn;
	
	if (isChandelier)
	{
		model_comp.setMaterial(&AssetManager::GetMaterial(lightOn ? "flame" : "chandelier_candle"), 2);
		model_comp.setMaterial(&AssetManager::GetMaterial(lightOn ? "flame" : "flame_off"), 3);
	}
	else
	{
		model_comp.setMaterial(&AssetManager::GetMaterial(lightOn ? "flame" : "flame_off"), 1);
	}
}

void LampComponent::update(float deltaTime)
{
	if (!compValid) return;

	if (reverse)
	{
		timer -= deltaTime;
		if (timer < 0.0f) reverse = false;
	}
	else
	{
		timer += deltaTime;
		if (timer > 0.0f) reverse = true;
	}

	PointLightComponent& light_comp = ECS::GetComponent(light);
	light_comp.diffuseStrength = baseLightIntensity + (Maths::sin(timer) / 11.0f);
}