#include "raycastAABB.h"

RaycastAABB::RaycastAABB(const Vector3& location, const Box& boxInfos, float drawDebugTime, bool loadPersistent) :
	Raycast(drawDebugTime, loadPersistent),
	drawDebugCube(), box(boxInfos)
{
	type = RaycastType::RaycastTypeAABB;

	box.setCenterPoint(boxInfos.getCenterPoint() + location);

	drawDebugCube.setBox(box);
}


void RaycastAABB::drawDebugRaycast(Material& debugMaterial)
{
	drawDebugCube.draw(debugMaterial, hit ? Color::red : Color::green);
}

void RaycastAABB::setHit()
{
	hit = true;
}
