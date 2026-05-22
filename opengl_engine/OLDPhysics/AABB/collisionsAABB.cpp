#include "collisionsAABB.h"
#include <Maths/maths.h>

using Maths::max;
using Maths::min;

bool CollisionsAABB::IntersectPoint(const BoxAABBColComp& boxAABB, const Vector3& point)
{
	Box box = boxAABB.getTransformedBox();
	return BoxPointIntersection(box, point);
}

bool CollisionsAABB::IntersectLineRaycast(const BoxAABBColComp& boxAABB, const Ray& raycast, RaycastHitInfos& outHitInfos)
{
	Box box = boxAABB.getTransformedBox();

	float hit_distance = 0.0f;
	Vector3 hit_location = Vector3::zero;

	bool intersect = BoxRayIntersection(box, raycast, hit_distance, hit_location);

	//  check if it is the closest collision found
	if (hit_distance < outHitInfos.hitDistance)
	{
		outHitInfos.hitDistance = hit_distance;
		outHitInfos.hitLocation = hit_location;
		outHitInfos.hitNormal = boxAABB.getNormal(hit_location);
		outHitInfos.hitCollision = &boxAABB;
	}

	return intersect;
}

bool CollisionsAABB::IntersectAABBRaycast(const BoxAABBColComp& boxAABB, const Box& raycast)
{
	Box box = boxAABB.getTransformedBox();

	bool intersect = BoxesIntersection(box, raycast);

	return intersect;
}

bool CollisionsAABB::IntersectAABBSweepRaycast(const BoxAABBColComp& boxAABB, const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, bool forCollisionTest)
{
	Box box = boxAABB.getTransformedBox();

	float hit_distance = 0.0f;
	Vector3 hit_location = Vector3::zero;

	bool intersect = CCDBoxIntersectionRaycast(boxRaycast, raycast, box, hit_distance, hit_location, forCollisionTest);

	//  check if it is trigger
	if(intersect && boxAABB.getCollisionType() == CollisionType::Trigger)
	{
		outHitInfos.triggersDetected.push_back(&boxAABB);
		return false;
	}

	//  check if it is the closest collision found
	if (intersect && hit_distance < outHitInfos.hitDistance)
	{
		outHitInfos.hitDistance = hit_distance;
		outHitInfos.hitLocation = hit_location;
		outHitInfos.hitNormal = box.getNearestFaceNormal(Box{ hit_location, boxRaycast.getHalfExtents() });
		outHitInfos.hitCollision = &boxAABB;
	}

	return intersect;
}


bool CollisionsAABB::BoxesIntersection(const Box& boxA, const Box& boxB)
{
	Box minkowski = Box::MinkowskiDifference(boxA, boxB);

	return BoxPointIntersection(minkowski, Vector3::zero);
}

bool CollisionsAABB::BoxPointIntersection(const Box& box, const Vector3& point)
{
	Vector3 box_min = box.getMinPoint();
	Vector3 box_max = box.getMaxPoint();

	return
		point.x >= box_min.x && point.x <= box_max.x &&
		point.y >= box_min.y && point.y <= box_max.y &&
		point.z >= box_min.z && point.z <= box_max.z;
}

bool CollisionsAABB::BoxRayIntersection(const Box& box, const Ray& ray, float& distance, Vector3& location, bool computeCollision)
{
	Vector3 box_min = box.getMinPoint();
	Vector3 box_max = box.getMaxPoint();

	Vector3 ray_origin = ray.getOrigin();
	Vector3 ray_direction = ray.getDirection();
	float ray_length = ray.getLength();

	Vector3 dirfrac;
	dirfrac.x = 1.0f / ray_direction.x;
	dirfrac.y = 1.0f / ray_direction.y;
	dirfrac.z = 1.0f / ray_direction.z;

	float t1 = (box_min.x - ray_origin.x) * dirfrac.x;
	float t2 = (box_max.x - ray_origin.x) * dirfrac.x;
	float t3 = (box_min.y - ray_origin.y) * dirfrac.y;
	float t4 = (box_max.y - ray_origin.y) * dirfrac.y;
	float t5 = (box_min.z - ray_origin.z) * dirfrac.z;
	float t6 = (box_max.z - ray_origin.z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	distance = std::numeric_limits<float>::max();

	//  if tmax == 0, origin of the ray is on the edge of AABB
	//  it is fine that it return true for most cases, but when using raycasts for computing collisions, it should return false
	if (tmax == 0.0f && computeCollision)
	{
		return false;
	}

	//  if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return false;
	}

	//  if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0.0f)
	{
		return false;
	}

	//  if tmin > length, ray (line) is intersecting AABB, but the whole AABB is after the end of the raycast
	if (tmin > ray_length)
	{
		return false;
	}

	//  if tmin > 0, origin of the ray is inside of AABB
	//  when computing collisions, it shouldn't return a location on the ray direction axis, but the nearest location outside of the box 
	if (tmin < 0.0f && computeCollision)
	{
		location = box.getPointOnPerimeter(ray_origin);
		distance = Vector3::Distance(ray_origin, location);
		
		return true;
	}

	distance = tmin;
	location = ray_origin + ray_direction * distance;

	return true;
}

bool CollisionsAABB::CCDBoxIntersectionRaycast(const Box& boxRaycast, const Ray& ray, const Box& boxObject, float& distance, Vector3& centerLocation, bool forCollisionTest)
{
	Vector3 box_raycast_pos = boxRaycast.getCenterPoint();

	if (box_raycast_pos == ray.getEnd()) //  raycast AABB Sweep but without the sweep (??)
	{
		bool intersect = BoxesIntersection(boxRaycast, boxObject);
		distance = 0.0f;
		centerLocation = boxRaycast.getCenterPoint();
		return intersect;
	}

	Box box_static = boxObject;
	box_static.addHalfExtents(boxRaycast);

	bool intersect = BoxRayIntersection(box_static, ray, distance, centerLocation, forCollisionTest);

	return intersect;
}
