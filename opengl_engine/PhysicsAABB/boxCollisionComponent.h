#pragma once
#include <ECS/component.h>
#include <Maths/Geometry/box.h>
#include <Events/event.h>
#include "raycastUtils.h"
#include <string>

class RigidbodyComponent;


/** Box Collision Component
* A component that holds informations to add an AABB box collision to an entity.
*/
class BoxCollisionComponent : public Component
{
public:
	/** If true, the collision will not block rigidbodies and will either trigger an event. */
	bool isTrigger{ false };

	/** The collision channel of this collision component. If empty, will only be intersected by raycasts and rigidbodies that test everything. */
	std::string collisionChannel{ "" };

	/** The box values of this collision component. Note: it scales by default with the entity's transform, but it can be disabled with parameters. */
	Box collisionBox{ Box::one };

	/** Does the scale of the entity owning this collision component affects the box size. */
	bool useEntityScaleForBoxSize{ true };

	/** Does the scale of the entity owning this collision component affects the box center (if the box center is not Vector3::zero). */
	bool useEntityScaleForBoxCenter{ true };


	/** 
	* Event triggered if this collision is intersected by a raycast. 
	* Param A is the type of raycast intersected.
	* Param B is the location of the raycast hit.
	*/
	Event<RaycastType, const Vector3&> onRaycastIntersect;

	/**
	* Event triggered if this collision is collided by a rigidbody.
	* Param A is the rigidbody collided.
	* Param B is the collision normal.
	*/
	Event<const RigidbodyComponent&, const Vector3&> onCollisionIntersect;

	/**
	* Event triggered if this collision is set to trigger and if a rigidbody entered it.
	* Param A is the rigidbody triggered.
	*/
	Event<const RigidbodyComponent&> onTriggerEnter;


	/** Returns the world-space box of this collision, transformed by the entity owning this collision. */
	Box getTransformedBox() const;

	/** Returns the center down position of the world-space box of this collision. */
	Vector3 getCenterDownPos() const;

	/** Get the rigidbody owning this collision. Return an invalid handle if no rigidbody owns it. */
	ComponentHandle<RigidbodyComponent> getOwningRigidbody() const;

	
	/** DON'T SET MANUALLY. Debug value set to true for a frame if the collision has been intersected by a rigidbody. */
	bool debugIntersectedLastFrame{ false };


private:
	friend class RigidbodyComponent;

	/** The rigidbody owning this collision. Null if no rigidbody owns it. */
	ComponentHandle<RigidbodyComponent> owningRigidbody;
};


// Specify sublist size for 'BoxCollisionComponent'
template<>
struct ComponentSublistSize<BoxCollisionComponent>
{
	static constexpr size_t value = 32;
};