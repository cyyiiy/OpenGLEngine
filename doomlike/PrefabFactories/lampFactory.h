#pragma once
#include <ECS/entity.h>

class EntityContainer;


namespace LampFactory
{
	Entity* CreateLamp(EntityContainer* entityContainer, const Vector3& position, float intensityMultiplier, bool isCeiling, bool startOff = false);

	void SetupLampAssets();
}

