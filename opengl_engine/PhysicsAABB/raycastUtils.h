#pragma once
#include <Maths/Vector3.h>
#include <ECS/ecsTypes.h>
#include <vector>
#include <limits>

class BoxCollisionComponent;


enum class RaycastType : uint8_t
{
	RaycastTypeNone = 0,
	RaycastTypeLine = 1,
	RaycastTypeAABB = 2,
	RaycastTypeAABBSweep = 3
};


struct RaycastHitInfos
{
	RaycastHitInfos(Vector3 location, Vector3 normal, float distance, ComponentHandle<BoxCollisionComponent> collision, std::vector<ComponentHandle<BoxCollisionComponent>> triggers) :
		hitLocation(location), hitNormal(normal), hitDistance(distance), hitCollision(collision), triggersDetected(triggers) {
	}

	RaycastHitInfos() :
		hitLocation(Vector3::zero), hitNormal(Vector3::zero), hitDistance(std::numeric_limits<float>::max()), hitCollision(), triggersDetected({}) {
	}

	Vector3 hitLocation{ Vector3::zero };
	Vector3 hitNormal{ Vector3::zero };
	float hitDistance{ std::numeric_limits<float>::max() }; // Initialized to max float to compute the nearest hit in case of multiple hits
	ComponentHandle<BoxCollisionComponent> hitCollision;
	std::vector<ComponentHandle<BoxCollisionComponent>> triggersDetected;

	static RaycastHitInfos defaultInfos;
};