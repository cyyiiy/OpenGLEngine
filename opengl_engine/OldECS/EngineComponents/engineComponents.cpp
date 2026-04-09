#include "engineComponents.h"
#include <ECS/componentManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/Lights/spotLightComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Hud/spriteRendererComponent.h>
#include <Rendering/Text/textRendererComponent.h>

#include <Physics/AABB/boxAABBColComp.h>
#include <Physics/rigidbodyComponent.h>

#include <Audio/audioSourceComponent.h>


void EngineComponents::RegisterEngineComponents()
{
	ComponentManager::RegisterComponentDataByClass<CameraComponent>(ComponentClassData{ false, 10 });
	ComponentManager::RegisterComponentDataByClass<ModelRendererComponent>(ComponentClassData{ false, 100 });
	ComponentManager::RegisterComponentDataByClass<PointLightComponent>(ComponentClassData{ false, 50 });
	ComponentManager::RegisterComponentDataByClass<SpotLightComponent>(ComponentClassData{ false, 50 });
	ComponentManager::RegisterComponentDataByClass<DirectionalLightComponent>(ComponentClassData{ false, 5 });
	ComponentManager::RegisterComponentDataByClass<SpriteRendererComponent>(ComponentClassData{ false, 50 });
	ComponentManager::RegisterComponentDataByClass<TextRendererComponent>(ComponentClassData{ false, 50 });

	ComponentManager::RegisterComponentDataByClass<BoxAABBColComp>(ComponentClassData{ false, 100 });
	ComponentManager::RegisterComponentDataByClass<RigidbodyComponent>(ComponentClassData{ false, 50 });

	ComponentManager::RegisterComponentDataByClass<AudioSourceComponent>(ComponentClassData{ false, 50 });
}
