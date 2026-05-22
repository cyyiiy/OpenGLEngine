#pragma once
#include <Physics/raycast.h>

#include <Maths/Geometry/box.h>
#include <Rendering/Debug/cube.h>


/** Raycast AABB
* A type of raycast that check for colliders with the shape of a static AABB Box.
*/
class RaycastAABB : public Raycast
{
public:
	RaycastAABB(const Vector3& location, const Box& boxInfos, float drawDebugTime, bool loadPersistent = false);
	RaycastAABB() = delete;
	RaycastAABB(const RaycastAABB&) = delete;
	RaycastAABB& operator=(const RaycastAABB&) = delete;

	void drawDebugRaycast(Material& debugMaterial) override;

	void setHit();

	inline const Box& getBox() const { return box; }

private:
	Box box;

	Cube drawDebugCube;
};

