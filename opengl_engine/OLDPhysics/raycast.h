#pragma once

#include <Maths/vector3.h>
#include <limits>
#include <vector>

class CollisionComponent;
class Material;


enum class RaycastType : uint8_t
{
	RaycastTypeNone = 0,
	RaycastTypeLine = 1,
	RaycastTypeAABB = 2,
	RaycastTypeAABBSweep = 3
};


struct RaycastHitInfos
{
	RaycastHitInfos(Vector3 location, Vector3 normal, float distance, const CollisionComponent* collision, std::vector<const CollisionComponent*> triggers) :
		hitLocation(location), hitNormal(normal), hitDistance(distance), hitCollision(collision), triggersDetected(triggers) {}

	RaycastHitInfos() :
		hitLocation(Vector3::zero), hitNormal(Vector3::zero), hitDistance(std::numeric_limits<float>::max()), hitCollision(nullptr), triggersDetected({}) {}

	Vector3 hitLocation{ Vector3::zero };
	Vector3 hitNormal{ Vector3::zero };
	float hitDistance{ std::numeric_limits<float>::max() }; //  used to get the nearest hit in case of multiple hits
	const CollisionComponent* hitCollision{ nullptr };
	std::vector<const CollisionComponent*> triggersDetected;

	static RaycastHitInfos defaultInfos;
};


/** Raycast
* Raycast class contains common functionnalities to all raycasts types, such as timer and draw.
*/
class Raycast 
{
public:
	Raycast(float drawDebugTime, bool loadPersistent = false);
	Raycast() = delete;
	Raycast(const Raycast&) = delete;
	Raycast& operator=(const Raycast&) = delete;

	virtual void drawDebugRaycast(Material& debugMaterial) = 0;

	void updateDrawDebugTimer(float dt);

	inline bool drawDebugTimerFinished() const { return drawDebugTimer == 0.0f; }
	inline bool isLoadedPersistent() const { return loadedPersistent; }

	inline RaycastType getRaycastType() const { return type; }

protected:
	bool hit{ false };

	RaycastType type{ RaycastType::RaycastTypeNone };

private:
	float drawDebugTimer;
	bool loadedPersistent;
};

