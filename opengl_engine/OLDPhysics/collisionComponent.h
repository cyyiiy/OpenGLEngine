#pragma once
#include <ECS/component.h>
#include <Physics/raycast.h>

#include <Maths/Geometry/box.h>
#include <Maths/Geometry/ray.h>
#include <Maths/Vector3.h>
#include <Maths/Matrix4.h>

#include <Events/event.h>
#include <Audio/audioUtils.h>

#include <memory>
#include <vector>


class RigidbodyComponent;
class Material;
class Mesh;



enum class CollisionShape : uint8_t
{
	Null = 0,
	BoxAABB = 1
};

enum class CollisionType : uint8_t
{
	Solid = 0,
	Trigger = 1
};




/** Collision Component
* Base class for every collision components.
* Abstract Component - do not add it to an Entity.
*/
class CollisionComponent : public Component
{
public:
	virtual ~CollisionComponent() {}


// ----------------------------------------------------------
//  Raycast Tests
// ----------------------------------------------------------
public:
	/**
	* Check if a point in space is inside this collision.
	* @param	point		The position to check.
	* @return				True if the point is inside this collision, false otherwise.
	*/
	bool resolvePoint(const Vector3& point) const;
	
	/**
	* Check if a line raycast intersect this collision.
	* @param	raycast			The line raycast to check.
	* @param	outHitInfos		Informations on the raycast intersection with this collision. [OUT]
	* @param	testChannels	The collision channels tested by the raycast.
	* @return					True if the raycast intersect this collision, false otherwise.
	*/
	bool resolveLineRaycast(const Ray& raycast, RaycastHitInfos& outHitInfos, const std::vector<std::string> testChannels) const;
	
	/**
	* Check if an AABB raycast intersect this collision.
	* Note that out hit infos does not exist for this type of raycast (for the moment).
	* @param	raycast			The AABB raycast to check.
	* @param	testChannels	The collision channels tested by the raycast.
	* @return					True if the raycast intersect this collision, false otherwise.
	*/
	bool resolveAABBRaycast(const Box& raycast, const std::vector<std::string> testChannels) const;
	
	/**
	* Check if an AABB sweep raycast intersect this collision.
	* @param	raycast				The line to sweep the AABB raycast on.
	* @param	boxRaycast			The AABB of the sweep raycast to check.
	* @param	outHitInfos			Informations on the raycast intersection with thi collision. [OUT]
	* @param	testChannels		The collision channels tested by the raycast.
	* @param	forCollisionTest	Is this raycast used to compute collisions?
	* @return						True if the raycast intersect this collision, false otherwise.
	*/
	bool resolveAABBSweepRaycast(const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, const std::vector<std::string> testChannels, bool forCollisionTest = false) const;


	/**
	* Set the collision type of this collision (either solid or trigger).
	* @param	newCollisionType	The new collision type to set to this collision.
	*/
	void setCollisionType(CollisionType newCollisionType);

	/** 
	* Set the collision channel of this collision.
	* Note that if the component has no specified collision channel (null string), it can be intersected only by raycasts that test everything.
	* @param	newCollisionChannel		The new collision channel to set to this collision.
	*/
	void setCollisionChannel(const std::string& newCollisionChannel);

	/**
	* Test collision channels on this collision.
	* @param	testChannels	The collision channels to test.
	* @return					True if the collision channel of this collision is being tested.
	*/
	bool channelTest(const std::vector<std::string> testChannels) const;


// ----------------------------------------------------------
//  Collision getters
// ----------------------------------------------------------
public:
	CollisionShape getCollisionShape() const;
	CollisionType getCollisionType() const;
	std::string getCollisionChannel() const;

	virtual const Matrix4 getModelMatrix() const;
	virtual Vector3 getCenterDownPos() const;
	virtual Box getEncapsulatingBox() const;


// ----------------------------------------------------------
//  Rigidbody
// ----------------------------------------------------------
public:
	/**
	* Set this collision owned by a rigidbody.
	* If this function is called with nullptr as a paremeter, it will set this collision as not used by a rigidbody (default state).
	* @param	rigidbodyOwner		The rigidbody that will own this collision.
	*/
	void setOwningRigidbody(RigidbodyComponent* rigidbodyOwner);

	/**
	* Get the rigidbody that owns this collision.
	* @return	The pointer to the owner rigidbody if there is one, nullptr otherwise.
	*/
	RigidbodyComponent* getOwningRigidbody() const;

	/** 
	* Know if this collision is owned by a rigidbody.
	* @return	True if this collision is owned by a valid rigidbody.
	*/
	bool ownedByRigidbody() const;


// ----------------------------------------------------------
//  Debug Drawing
// ----------------------------------------------------------
public:
	void drawDebug(Material& debugMaterial);
	void setDebugIntersected(bool debugIntersected) const; //  const cause the Physics Manager need to call this function on RaycastHitInfos that contains a pointer to a const Collision Component.
	bool getDebugIntersected() const;


// ----------------------------------------------------------
//  Audio Collision (wip, not working for the moment)
// ----------------------------------------------------------
public:
	/**
	* Setup this collision as an audio collision.
	* Warning: Audio Collision is not working in the engine for the moment. It will most likely have no effect.
	* @param	AudioCollisionOcclusion		The datas for the audio collision.
	*/
	virtual void setupAudioCollision(const AudioCollisionOcclusion& audioCollisionType) {}


// ----------------------------------------------------------
//  Events
// ----------------------------------------------------------
public:
	Event<> onCollisionDelete;
	mutable Event<RaycastType, const Vector3&> onRaycastIntersect;
	mutable Event<RigidbodyComponent&, const struct CollisionResponse&> onCollisionIntersect;
	mutable Event<RigidbodyComponent&> onTriggerEnter;



// ----------------------------------------------------------
//  Component registering and exit functions
// ----------------------------------------------------------
protected:
	virtual void registerComponent() override;
	virtual void unregisterComponent() override;

	void exit() override;

	void resetValues();


// ----------------------------------------------------------
//  Virtual functions for child collision components
// ----------------------------------------------------------
protected:
	virtual bool resolvePointIntersection(const Vector3& point) const = 0;
	virtual bool resolveLineRaycastIntersection(const Ray& raycast, RaycastHitInfos& outHitInfos) const = 0;
	virtual bool resolveAABBRaycastIntersection(const Box& raycast) const = 0;
	virtual bool resolveAABBSweepRaycastIntersection(const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, bool forCollisionTest) const = 0;

	virtual void drawDebugMesh(Material& debugMaterial) const = 0;


// ----------------------------------------------------------
//  Properties accessible from child collision components
// ----------------------------------------------------------
protected:
	CollisionShape collisionShape{ CollisionShape::Null };
	CollisionType collisionType{ CollisionType::Solid };

	bool isAudioCollision{ false };
	std::uint32_t audioCollisionIndex{ 0 };

	Mesh* debugMesh{ nullptr };



// ----------------------------------------------------------
//  Properties hidden to child collision components
// ----------------------------------------------------------
private:
	std::string collisionChannel{ "" };

	mutable bool debugIntersectedLastFrame{ false };

	RigidbodyComponent* owningRigidbody{ nullptr };
};