#pragma once
#include "boxAABBColComp.h"
#include <Physics/rigidbodyComponent.h>

/** Collisions AABB
* Static functions that will check collisions that involves AABB components.
*/
class CollisionsAABB
{
public:
	static bool IntersectPoint(const BoxAABBColComp& boxAABB, const Vector3& point);

	static bool IntersectLineRaycast(const BoxAABBColComp& boxAABB, const Ray& raycast, RaycastHitInfos& outHitInfos);

	static bool IntersectAABBRaycast(const BoxAABBColComp& boxAABB, const Box& raycast);

	static bool IntersectAABBSweepRaycast(const BoxAABBColComp& boxAABB, const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, bool forCollisionTest);


private:
	static bool BoxesIntersection(const Box& boxA, const Box& boxB);

	static bool BoxPointIntersection(const Box& box, const Vector3& point);

	static bool BoxRayIntersection(const Box& box, const Ray& ray, float& distance, Vector3& location, bool computeCollision = false);

	static bool CCDBoxIntersectionRaycast(const Box& boxRaycast, const Ray& ray, const Box& boxObject, float& distance, Vector3& centerLocation, bool forCollisionTest);
};

