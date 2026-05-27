#pragma once
#include <ServiceLocator/physics.h>

/**
* The physics null service provider class.
*/
class NullPhysics : public Physics
{
public:
	bool LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene) override { return false; }
	bool AABBRaycast(const Vector3& location, const Box& aabbBox, const std::vector<std::string> testChannels, float drawDebugTime, bool createOnScene) override { return false; }
	bool AABBSweepRaycast(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene) override { return false; }
	//bool AABBSweepPhysicTest(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, const CollisionComponent* testedCol, RaycastHitInfos& outHitInfos) override { return false; }

	//float GetGravityValue() override { return 0.0f; }

	void SetEnableInfoLogs(bool enable) override {}


	void UpdatePhysics(float dt) override {}
	void ClearRaycastOnSceneUnload(bool exitGame) override{}
};