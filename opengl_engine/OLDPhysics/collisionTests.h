#pragma once
#include <Maths/Vector3.h>
#include <vector>

class RigidbodyComponent;
class CollisionComponent;
class Box;

struct CollisionHit
{
	const CollisionComponent& collisionComponent;
	const Vector3 impactPoint;
	const Vector3 impactNormal;
};

/**
* Static functions that will test collisions for rigidbodies.
*/
class CollisionTests
{
public:

	/**
	* Compute the movement by checking collisions for a physic activated rigidbody.
	* Algorithm used is Collide and Slide.
	* @param	rigidbody			Rigidbody to compute.
	* @param	gravityPass			Are we computing gravity?
	* @param	computedMovement	Movement to apply to the rigidbody. [OUT]
	* @param	colResponses		List of all collisions hit. [OUT]
	* @param	triggers			List of all triggers detected. [OUT]
	* @return						True if the rigidbody encountered at least one collision.
	*/
	static bool RigidbodyCollideAndSlideAABB(const RigidbodyComponent& rigidbody, const bool gravityPass, Vector3& computedMovement, std::vector<CollisionHit>& colResponses, std::vector<const CollisionComponent*>& triggers);


private:

	/**
	* Core of the Collide and Slide algorithm, recursive function that will check the shape raycast.
	* @param	rigidbody			Rigidbody to use.
	* @param	colComp				Collision component to use
	* @param	boxAABB				Shape to use.
	* @param	startPos			Start of the raycast for this iteration.
	* @param	movement			Movement to check for this iteration.
	* @param	bounces				Iteration number.
	* @param	gravityPass			Are we computing gravity?
	* @param	computedPos			End of the raycast for this iteration. [OUT]
	* @param	colResponses		List of all collisions hit. [OUT/PASSTHROUGH]
	* @param	triggers			List of all triggers detected. [OUT/PASSTHROUGH]
	* @return						True if this iteration's raycast encountered a collision.
	*/
	static bool CollideAndSlideAABB(const RigidbodyComponent& rigidbody, const CollisionComponent* colComp, const Box& boxAABB, const Vector3 startPos, const Vector3 movement, const int bounces, const bool gravityPass, Vector3& computedPos, std::vector<CollisionHit>& colResponses, std::vector<const CollisionComponent*>& triggers);
};

