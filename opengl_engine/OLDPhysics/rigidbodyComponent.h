#pragma once
#include <ECS/component.h>
#include <Physics/collisionComponent.h>

#include <Maths/Vector3.h>

#include <Events/event.h>
#include <Events/observer.h>

#include <memory>
#include <vector>
#include <string>


namespace Rigidbody
{
	const int MAX_BOUNCES = 5;
	const float SECURITY_DIST = 0.001f;
}


struct CollisionResponse
{
	Vector3 impactPoint{ Vector3::zero };
	Vector3 impactNormal{ Vector3::zero };
};


/** Rigidbody Component
* Component that can be associated with a Collision Component on an Entity.
* If the Rigidbody have physics activated, it will have gravity and will test and be blocked by other collisions.
* If physics is not activated, it will act as a static collision and won't test other collisions but will still be able to move with velocity.
* Repulsion between two physics activated Rigidbodies is not implemented for the moment, but the collision detection (trigger) will still happen.
*/
class RigidbodyComponent : public Component, public Observer
{
// ----------------------------------------------------------
//  Associated Collision
// ----------------------------------------------------------
public:
	/**
	* Set the associated collision of this rigidbody.
	* @param	collisionToAssociate	The collision that this rigidbody must manage.
	*/
	void associateCollision(std::weak_ptr<CollisionComponent> collisionToAssociate);

	/**
	* Know if the collision managed by this rigidbody is valid, ie. if this rigidbody is valid.
	* @return	True if this rigidbody manage a valid collision.
	*/
	bool isAssociatedCollisionValid() const;

	/**
	* Get a const reference to the collision managed by this rigidbody.
	* @return	The const reference to the associated collision.
	*/
	const CollisionComponent& getAssociatedCollision() const;

	/**
	* Get a non-const reference to the collision managed by this rigidbody.
	* @return	The non-const reference to the associated collision.
	*/
	CollisionComponent& getAssociatedCollisionNonConst();


// ----------------------------------------------------------
//  Rigidbody Parameters
// ----------------------------------------------------------
public:
	/** Set this Rigidbody as physics activated or not. Physics activated Rigidbodies will test collisions. */
	void setPhysicsActivated(bool value);

	/** Set this Rigidbody to use gravity or not. */
	void setUseGravity(bool value);

	/** Set the step height of this Rigidbody, to automatically step over collisions when moving with velocity (only works if physics is activated). */
	void setStepHeight(float value);


	/** Know if this Rigidbody is physics activated. Note that if it doesn't manage a valid collision, it will return false. */
	bool isPhysicsActivated() const;

	/** Get the value of 'use gravity' of this Rigidbody. */
	bool getUseGravity() const;

	/** Get the value of 'step height' of this Rigidbody. */
	float getStepHeight() const;


// ----------------------------------------------------------
//  Velocity
// ----------------------------------------------------------
public:
	/**
	* Set the velocity of this Rigidbody.
	* @param	value	The velocity value to set.
	*/
	void setVelocity(const Vector3& value);

	/**
	* Increment the velocity of this Rigidbody.
	* @param	value	The velocity value to add.
	*/
	void addVelocity(const Vector3& value);

	/**
	* Increment the velocity of this Rigidbody, only for one frame.
	* @param	value	The velocity value to add for one frame.
	*/
	void addVelocityOneFrame(const Vector3& value);

	/**
	* Get the velocity of this Rigidbody.
	* @return	The velocity of this Rigidbody.
	*/
	Vector3 getVelocity() const;

	/**
	* Get the movement that is anticipated by this Rigidbody for the current frame.
	* Note that this function will work only during the Physics part of the update.
	* This function is mainly useful for the collision tests of the Physics computations.
	* @return	The anticipated movement for the current frame.
	*/
	Vector3 getAnticipatedMovement() const;


// ----------------------------------------------------------
//  Gravity Velocity
// ----------------------------------------------------------
public:
	/**
	* Set the gravity velocity of this Rigidbody.
	* @param	value	The gravity velocity value to set.
	*/
	void setGravityVelocity(const Vector3& value);

	/**
	* Increment the gravity velocity of this Rigidbody.
	* @param	value	The gravity velocity value to add.
	*/
	void addGravityVelocity(const Vector3& value);

	/**
	* Get the gravity velocity of this Rigidbody.
	* @return	The gravity velocity of this Rigidbody.
	*/
	Vector3 getGravityVelocity() const;

	/**
	* Get the gravity movement that is anticipated by this Rigidbody for the current frame.
	* Note that this function will work only during the Physics part of the update.
	* This function is mainly useful for the collision tests of the Physics computations.
	* @return	The anticipated gravity movement for the current frame.
	*/
	Vector3 getAnticipatedGravityMovement() const;


// ----------------------------------------------------------
//  Other Rigidbody functions
// ----------------------------------------------------------
public:
	/**
	* Check the step mechanic on a collision for this Rigidbody.
	* The step mechanic allows a physics activated Rigidbody to step over collisions that are low enough.
	* @param	collidedComp		The collision component to compute the step mechanic on.
	* @param	aimedDestination	The position that this rigidbody want to reach but blocked by the collision component.
	* @param	hitNormal			The normal vector of the surface hit on the collision component.
	* @param	stepMovement		[OUT] The computed vertical movement needed to reach the aimed destination on top of the collision component.
	* @return						True if the step mechanic is valid for this situation, false otherwise.
	*/
	bool checkStepMechanic(const CollisionComponent& collidedComp, const Vector3 aimedDestination, const Vector3 hitNormal, float& stepMovement) const;

	/**
	* Know if this Rigidbody is on the ground.
	* @return	True if this Rigidbody uses gravity and if it is on the ground.
	*/
	bool isOnGround() const;


//  Observer functions
private:
	void onCollisionIntersected(RigidbodyComponent& other, const CollisionResponse& collisionResponse);
	void onCollision(const CollisionResponse& collisionResponse);


// ----------------------------------------------------------
//  Rigibody Test Channels
// ----------------------------------------------------------
public:
	/** Set the collision channels that this Rigidbody will test when updating physics. */
	void setTestChannels(std::vector<std::string> newTestChannels);

	/** Add a collision channel that this Rigidbody will test when updating physics. */
	void addTestChannel(std::string newTestChannel);

	/** Get the collision channels that this Rigidbody test when updating physics. */
	std::vector<std::string> getTestChannels() const;


// ----------------------------------------------------------
//  Events
// ----------------------------------------------------------
public:
	Event<> onRigidbodyDelete;
	Event<const CollisionResponse&> onCollisionRepulsed;



// ----------------------------------------------------------
//  Component registering functions
// ----------------------------------------------------------
protected:
	virtual void registerComponent() override;
	virtual void unregisterComponent() override;

	virtual void init() override;
	virtual void exit() override;


// ----------------------------------------------------------
//  Rigidbody properties
// ----------------------------------------------------------
private:
	std::shared_ptr<CollisionComponent> associatedCollision{ nullptr };

	bool physicsActivated{ false };
	bool useGravity{ false };
	float stepHeight{ 0.0f };

	Vector3 velocity{ Vector3::zero };
	Vector3 velocityOneFrame{ Vector3::zero };
	Vector3 movement{ Vector3::zero };

	Vector3 gravityVelocity{ Vector3::zero };
	Vector3 gravityMovement{ Vector3::zero };

	bool onGround{ false };
	bool groundedLastFrame{ false };
	bool firstFrame{ true };

	std::vector<std::string> testChannels;



// ----------------------------------------------------------
//  Update Physics functions for the Physics Manager
// ----------------------------------------------------------
private:
	friend class PhysicsManager;

	/**
	* Update the physics of this Rigidbody before the collision computation.
	* @param	dt		Delta time of the current frame.
	*/
	void updatePhysicsPreCollision(float dt);

	/**
	* Update the physics of this Rigidbody after the collision computation.
	* @param	dt		Delta time of the current frame.
	*/
	void updatePhysicsPostCollision(float dt);

	/**
	* Apply the physics-computed movement of this Rigidbody for the current frame.
	* @param	computedMovement	The movement computed with physics of this Rigidbody.
	*/
	void applyComputedMovement(const Vector3& computedMovement);

	/**
	* Apply the physics-computed gravity movement of this Rigidbody for the current frame.
	* @param	computedGravityMovement		The gravity movement computed with physics of this Rigidbody.
	*/
	void applyComputedGravityMovement(const Vector3& computedGravityMovement);
};