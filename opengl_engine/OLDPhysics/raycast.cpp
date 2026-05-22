#include "raycast.h"

RaycastHitInfos RaycastHitInfos::defaultInfos(Vector3::zero, Vector3::zero, std::numeric_limits<float>::max(), nullptr, {});


Raycast::Raycast(float drawDebugTime, bool loadPersistent) : drawDebugTimer(drawDebugTime), loadedPersistent(loadPersistent)
{
}

void Raycast::updateDrawDebugTimer(float dt)
{
	if (drawDebugTimer <= 0.0f) return;

	drawDebugTimer -= dt;
	if (drawDebugTimer < 0.0f) drawDebugTimer = 0.0f;
}
