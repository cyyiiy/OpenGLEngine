#pragma once
#include <PhysicsAABB/raycastUtils.h>

struct Vector3;
//class Box;
//class Material;


/**
* The Physics Service class (the virtual class provided by the Locator).
*/
class Physics
{
public:
	virtual ~Physics() {}


	/**
	* Creates a line-shaped raycast between two points.
	* @param	start			Start point of the raycast (world coordinates).
	* @param	end				End point of the raycast (world coordinates).
	* @param	testChannels	Collision channels the raycast will test. If the vector is empty, it will test everything.
	* @param	outHitInfos		Informations on the closest encountered collision.
	* @param	drawDebugTime	Duration of the raycast debug draw. (0 = no draw debug, negative = infinite draw debug).
	* @param	createOnScene	Is the raycast registered on the scene (delete if scene change) or on the game (persist when changing scene).
	* @return					True if at least one collision intersect the line raycast.
	*/
	virtual bool LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels = {}, RaycastHitInfos& outHitInfos = RaycastHitInfos::defaultInfos, float drawDebugTime = 5.0f, bool createOnScene = true) = 0;

	/**
	* Creates an AABB box-shaped raycast at a location.
	* @param	location		Location of the raycast (world coordinates).
	* @param	aabbBox			Box shape of the raycast.
	* @param	testChannels	Collision channels the raycast will test. If the vector is empty, it will test everything.
	* @param	drawDebugTime	Duration of the raycast debug draw. (0 = no draw debug, negative = infinite draw debug).
	* @param	createOnScene	Is the raycast registered on the scene (delete if scene change) or on the game (persist when changing scene).
	* @return					True if at least one collision intersect the aabb box raycast.
	*/
	//virtual bool AABBRaycast(const Vector3& location, const Box& aabbBox, const std::vector<std::string> testChannels = {}, float drawDebugTime = 5.0f, bool createOnScene = true) = 0;

	/**
	* Sweep an AABB box-shaped raycast between two points.
	* @param	start				Start point of the sweep (world coordinates).
	* @param	end					End point of the sweep (world coordinates).
	* @param	aabbBox				Box shape of the raycast.
	* @param	testChannels		Collision channels the raycast will test. If the vector is empty, it will test everything.
	* @param	outHitInfos			Informations on the closest encountered collision.
	* @param	drawDebugTime		Duration of the raycast debug draw. (0 = no draw debug, negative = infinite draw debug).
	* @param	createOnScene		Is the raycast registered on the scene (delete if scene change) or on the game (persist when changing scene).
	* @return						True if at least one collision intersect the sweeped aabb box raycast.
	*/
	//virtual bool AABBSweepRaycast(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels = {}, RaycastHitInfos& outHitInfos = RaycastHitInfos::defaultInfos, float drawDebugTime = 5.0f, bool createOnScene = true) = 0;

	/**
	* Sweep an AABB box-shaped raycast between two points. Variant for the physic test.
	* @param	start				Start point of the sweep (world coordinates).
	* @param	end					End point of the sweep (world coordinates).
	* @param	aabbBox				Box shape of the raycast.
	* @param	testChannels		Collision channels the raycast will test. If the vector is empty, it will test everything.
	* @param	testedCol			The collision component that is currently tested for the physic.
	* @param	outHitInfos			Informations on the closest encountered collision.
	* @return						True if at least one collision intersect the sweeped aabb box raycast.
	*/
	//virtual bool AABBSweepPhysicTest(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, const CollisionComponent* testedCol, RaycastHitInfos& outHitInfos) = 0;

	/**
	* Retrieve the gravity strength.
	* @return	Gravity value.
	*/
	//virtual float GetGravityValue() = 0;


	/**
	* Set if the physics can log infos.
	* @param	enable		Enable state of info logging.
	*/
	//virtual void SetEnableInfoLogs(bool enable) = 0;


	/** Physics system update. Automatically called by the engine each frame. */
	virtual void UpdatePhysics(float dt) = 0;

	/** Instantly delete all scene-registered raycast renderer components. Automatically called by the game system on scene unload. */
	virtual void ClearRaycastOnSceneUnload(bool exitGame) = 0;
};