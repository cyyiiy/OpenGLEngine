#pragma once
#include <Physics/raycast.h>

#include <Maths/Geometry/ray.h>
#include <Maths/Geometry/box.h>

#include <Rendering/Debug/line.h>
#include <Rendering/Debug/cube.h>


/** Raycast AABB Sweep
* A type of raycast that sweep an AABB Box from a point A to a point B in a linear way.
* Warning: Box center offset will not be take into account, only it size will matter.
*/
class RaycastAABBSweep : public Raycast
{
public:
	RaycastAABBSweep(const Vector3& startPoint, const Vector3& endPoint, const Box& boxInfos, float drawDebugTime, bool loadPersistent = false);
	RaycastAABBSweep() = delete;
	RaycastAABBSweep(const RaycastAABBSweep&) = delete;
	RaycastAABBSweep& operator=(const RaycastAABBSweep&) = delete;

	void drawDebugRaycast(Material& debugMaterial) override;

	void setValues(bool raycastHit, Vector3 hitPosition);

	inline const Ray& getRay() const { return ray; }
	inline const Box& getBox() const { return box; }

private:
	Ray ray;
	Box box;

	Line drawDebugLineOne;
	Line drawDebugLineTwo;
	Cube drawDebugCubeOne;
	Cube drawDebugCubeTwo;
};