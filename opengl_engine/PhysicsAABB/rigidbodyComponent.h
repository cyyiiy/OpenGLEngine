#pragma once
#include <ECS/behaviorComponent.h>
#include "boxCollisionComponent.h"
#include <Events/event.h>
#include <Maths/Vector3.h>
#include <string>
#include <vector>


namespace Rigidbody
{
	const int MAX_BOUNCES = 5;
	const float SECURITY_DIST = 0.001f;
}


/** Rigidbody Component
* A component that holds informations to add a rigidbody to an entity.
* Note: a rigidbody must be associated with a collision, otherwise it won't be used.
* Note: collisions between two non-kinematic rigidbodies are not implemented. It will trigger an event.
*/
class RigidbodyComponent : public BehaviorComponent
{
public:
	/** A kinematic rigidbody will move with its velocity, but won't test collisions and can't use gravity. */
	bool isKinematic{ false };

	/** Does this rigidbody falls with gravity. */
	bool useGravity{ true };

	/** The collision channels tested by this rigidbody. If empty, it will test everything. */
	std::vector<std::string> collisionChannels;

	/** The maximum height this rigidbody can automatically step over collisions when moving with velocity. */
	float stepHeight{ 0.0f };

	/** The movement this rigidbody will perform. In meter per second. */
	Vector3 velocity{ Vector3::zero };

	/** The gravity movement this rigidbody will perform. In meter per second. */
	Vector3 gravityVelocity{ Vector3::zero };


	/**
	* Event triggered if this rigidbody collide a collision.
	* Param A is the collision collided.
	* Param B is the collision normal.
	*/
	Event<const BoxCollisionComponent&, const Vector3&> onCollisionRepulse;


private:
	/** The collision this rigidbody uses. If null, the rigidbody will be disabled. */
	ComponentHandle<BoxCollisionComponent> associatedCollision;

	/** Wether or not this rigidbody is on ground (when using gravity). */
	bool onGround{ false };

	/** True if this rigidbody was grounded last frame (use for reverse step mechanic). */
	bool groundedLastFrame{ false };

	/** An additional movement this rigidbody will perform, but used a single frame. */
	Vector3 velocityOneFrame{ Vector3::zero };

	/** The collision computed movement performed by this rigidbody. */
	Vector3 movement{ Vector3::zero };

	/** The collision computed gravity movement performed by this rigidbody. */
	Vector3 gravityMovement{ Vector3::zero };


	/** Internal value used to skip the movement application on the first frame of this rigidbody. */
	bool firstFrame{ true };

	/** Internal value set to true during the physic stage. */
	bool inPhysicComputation{ false };


public:
	void init() override;
	void exit() override;
	void update(float dt) override;


	/** Pre-compute the movements of this rigidbody before the collision computations. */
	void updatePhysicsPreCollision(float dt);

	/** Finish the movement update of this rigidbody after the collision computations. */
	void updatePhysicsPostCollision(float dt);

	/**
	* Check the step mechanic on a collision for this Rigidbody.
	* The step mechanic allows a physics activated Rigidbody to step over collisions that are low enough.
	* @param	collidedComp		The collision component to compute the step mechanic on.
	* @param	aimedDestination	The position that this rigidbody want to reach but blocked by the collision component.
	* @param	hitNormal			The normal vector of the surface hit on the collision component.
	* @param	stepMovement		[OUT] The computed vertical movement needed to reach the aimed destination on top of the collision component.
	* @return						True if the step mechanic is valid for this situation, false otherwise.
	*/
	bool checkStepMechanic(const BoxCollisionComponent& collidedComp, const Vector3 aimedDestination, const Vector3 hitNormal, float& stepMovement) const;

	/**
	* Check the grounded value after the gravity collision computations.
	* Also check if standing on top of a kinematic rigidbody.
	* Note: this function does nothing if called outside of the physics phase.
	*/
	void checkOnGround(const BoxCollisionComponent& groundCollision, const Vector3& impactNormal);


	/** Associate a collision to this rigidbody. */
	void associateCollision(ComponentHandle<BoxCollisionComponent> collision);

	/** Remove the association with the current associated collision of this rigidbody. */
	void removeAssociatedCollision();

	/** Get the collision associated with this rigidbody. */
	ComponentHandle<BoxCollisionComponent> getAssociatedCollision() const;

	/** Return true if this rigidbody is associated to a valid collision. */
	bool isAssociatedCollisionValid() const;

	
	/** Know if this rigidbody is currently on ground (when using gravity). */
	bool isOnGround() const;


	/** Get the pre-computed movement of this rigidbody. Only works during the physics phase. */
	Vector3 getTheoricalMovement() const;

	/** Get the pre-computed gravity movement of this rigidbody. Only works during the physics phase. */
	Vector3 getTheoricalGravityMovement() const;

	/** Know if this rigidbody hasn't live a full frame yet. */
	bool isFirstFrame() const;

	
	/** Add a one-frame duration velocity to this rigidbody. */
	void addVelocityOneFrame(const Vector3& velocityAdd);

	/** Apply collisions-computed movements to this rigidbody. Automatically called by the physics system. */
	void applyComputedMovements(const Vector3& computedMovement, const Vector3& computedGravityMovement);


private:
	/** Apply the computed movements to the entity and end the update of this rigidbody. */
	void applyMovementsToEntity();

	/**
	* Check the inverse step mechanic.
	* @param	stepMovement		[OUT] The computed vertical movement needed to reach the ground.
	* @return						True if the inverse step mechanic is valid for this situation, false otherwise.
	*/
	bool checkInverseStepMechanic(float& inverseStepMovement) const;
};