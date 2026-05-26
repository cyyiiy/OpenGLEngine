#pragma once
#include <ServiceLocator/physics.h>
#include "raycastUtils.h"
#include "raycastRendererComponent.h"
#include <Maths/Geometry/ray.h>
#include <vector>


/**
* The physics service provider class.
*/
class PhysicsManager : public Physics
{
public:
	bool LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene) override;

	void UpdatePhysics(float dt) override;
	void ClearRaycastOnSceneUnload(bool exitGame) override;

private:
	bool channelTest(const std::string& collisionChannel, const std::vector<std::string>& testChannels);
	bool resolveLineRaycast(const class BoxCollisionComponent& boxColComponent, const Ray& ray, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos);

	std::vector<ComponentHandle<RaycastRendererComponent>> sceneRaycasts;
};

