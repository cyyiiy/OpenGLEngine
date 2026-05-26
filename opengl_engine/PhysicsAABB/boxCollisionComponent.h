#pragma once
#include <ECS/component.h>
#include <Maths/Geometry/box.h>
#include <Events/event.h>
#include "raycastUtils.h"
#include <string>


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


	/** Event triggered if this collision is intersected by a raycast. */
	Event<RaycastType, const Vector3&> onRaycastIntersect;


	/** Returns the world-space box of this collision, transformed by the entity owning this collision. */
	Box getTransformedBox() const;

	/** Returns the center down position of the world-space box of this collision. */
	Vector3 getCenterDownPos() const;

	
	/** DON'T SET MANUALLY. Debug value set to true for a frame if the collision has been intersected by a rigidbody. */
	bool debugIntersectedLastFrame{ false };
};


// Specify sublist size for 'BoxCollisionComponent'
template<>
struct ComponentSublistSize<BoxCollisionComponent>
{
	static constexpr size_t value = 32;
};