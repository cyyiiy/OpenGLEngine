#pragma once
#include <ServiceLocator/physics.h>
#include "raycastUtils.h"
#include "raycastRendererComponent.h"
#include <Maths/Geometry/ray.h>
#include <vector>

class BoxCollisionComponent;
class RigidbodyComponent;


const Vector3 GRAVITY{ 0.0f, -9.8f, 0.0f };


/**
* The physics service provider class.
*/
class PhysicsManager : public Physics
{
public:
	bool LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene) override;
	bool AABBRaycast(const Vector3& location, const Box& aabbBox, const std::vector<std::string> testChannels, float drawDebugTime, bool createOnScene) override;
	bool AABBSweepRaycast(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene) override;
	bool AABBSweepPhysicTest(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, Entity* testedEntity, RaycastHitInfos& outHitInfos) override;

	const Vector3& GetGravityValue() override;
	void SetEnableInfoLogs(bool enable) override;
	void UpdatePhysics(float dt) override;
	void ClearRaycastOnSceneUnload(bool exitGame) override;

private:
	bool channelTest(const std::string& collisionChannel, const std::vector<std::string>& testChannels);
	bool resolveLineRaycast(const BoxCollisionComponent& boxColComponent, const Ray& ray, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos);
	bool resolveAABBRaycast(const BoxCollisionComponent& boxColComponent, const Box& box, const std::vector<std::string> testChannels, std::vector<ComponentHandle<BoxCollisionComponent>>& intersectedCols);
	bool resolveAABBSweepRaycast(const BoxCollisionComponent& boxColComponent, const Ray& ray, const Box& box, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, bool forCollisionTest);

	void testRigidbodyCollisions(RigidbodyComponent& rigidbodyComponent);

	std::vector<ComponentHandle<RaycastRendererComponent>> sceneRaycasts;

	bool enableInfoLog{ false };
};
