#pragma once
#include "raycast.h"

#include <Maths/Geometry/ray.h>
#include <Rendering/Debug/line.h>
#include <Rendering/Debug/point.h>


/** Raycast Line
* A type of raycast that goes from a point A to a point B in a linear way.
*/
class RaycastLine : public Raycast
{
public:
	RaycastLine(const Vector3& startPoint, const Vector3& endPoint, float drawDebugTime, bool loadPersistent = false);
	RaycastLine() = delete;
	RaycastLine(const RaycastLine&) = delete;
	RaycastLine& operator=(const RaycastLine&) = delete;

	void drawDebugRaycast(Material& debugMaterial) override;

	void setHitPos(Vector3 hitPosition);

	inline const Ray& getRay() const { return ray; }

private:
	Ray ray;

	Line drawDebugLineOne;
	Line drawDebugLineTwo;
	Point drawDebugPointHit;
};

