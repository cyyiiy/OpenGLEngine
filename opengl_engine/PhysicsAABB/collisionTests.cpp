#include "collisionTests.h"
#include "rigidbodyComponent.h"
#include "boxCollisionComponent.h"
#include <Maths/Geometry/box.h>
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>


bool CollisionTests::RigidbodyCollideAndSlideAABB(const RigidbodyComponent& rigidbody, const bool gravityPass, Vector3& computedMovement, std::vector<CollisionHit>& colResponses, std::vector<ComponentHandle<BoxCollisionComponent>>& triggers)
{
	const Vector3 body_start_movement = gravityPass ? rigidbody.getTheoricalGravityMovement() : rigidbody.getTheoricalMovement();
	if (body_start_movement == Vector3::zero)
	{
		computedMovement = body_start_movement;
		return false; // Do not check collisions if no movement
	}

	const BoxCollisionComponent& col_comp = ECS::GetComponent(rigidbody.getAssociatedCollision());
	Box body_shape = col_comp.getTransformedBox();
	const Vector3 body_start_pos = body_shape.getCenterPoint();
	body_shape.setCenterPoint(Vector3::zero);

	Vector3 computed_pos = Vector3::zero;

	bool col_hit = CollideAndSlideAABB(rigidbody, body_shape, body_start_pos, body_start_movement, 0, gravityPass, computed_pos, colResponses, triggers);
	computedMovement = computed_pos - body_start_pos;

	return col_hit;
}


bool CollisionTests::CollideAndSlideAABB(const RigidbodyComponent& rigidbody, const Box& boxAABB, const Vector3 startPos, const Vector3 movement, const int bounces, const bool gravityPass, Vector3& computedPos, std::vector<CollisionHit>& colResponses, std::vector<ComponentHandle<BoxCollisionComponent>>& triggers)
{
	// 1. Shoot a raycast to follow the movement
	RaycastHitInfos out_raycast;
	bool col_encountered = Locator::getPhysics().AABBSweepPhysicTest(startPos, startPos + movement, boxAABB, rigidbody.collisionChannels, rigidbody.getOwner(), out_raycast);
	
	// 2. Check for triggers
	if (!out_raycast.triggersDetected.empty())
	{
		for (auto& trigger : out_raycast.triggersDetected)
		{
			auto iter = std::find(triggers.begin(), triggers.end(), trigger);
			if (iter == triggers.end())
				triggers.push_back(trigger);
		}
	}

	if (col_encountered) // Collision encountered, continuing recursion
	{
		const BoxCollisionComponent& hit_collision = ECS::GetComponent(out_raycast.hitCollision);

		// 3. Check step mechanic
		float step_movement = 0.0f;
		if (rigidbody.checkStepMechanic(hit_collision, startPos + movement, out_raycast.hitNormal, step_movement))
		{
			computedPos = startPos + movement + Vector3{ 0.0f, step_movement, 0.0f };

			colResponses.push_back(CollisionHit{
				out_raycast.hitCollision,
				computedPos + Vector3::negUnitY * boxAABB.getHalfExtents(),
				Vector3::unitY
				});

			return true; // End the recursion if step mechanic was successful
		}

		// 4. Add the collision encountered to the list of collision responses
		colResponses.push_back(CollisionHit{ 
			out_raycast.hitCollision,
			out_raycast.hitLocation + (-out_raycast.hitNormal * boxAABB.getHalfExtents()), 
			out_raycast.hitNormal});

		// 5. Compute the end of movement location of this recursion step
		const Vector3 out_location_secure = out_raycast.hitLocation += -movement * Rigidbody::SECURITY_DIST;

		// 6. End the recursion if the maximum number of bounces is reached
		if (bounces > Rigidbody::MAX_BOUNCES)
		{
			computedPos = out_location_secure;
			return true;
		}

		// 7. Compute the available remaining movement magnitude
		float remaining_magnitude = movement.length() - out_raycast.hitDistance;

		// 8. Check the collision angle
		if (out_raycast.hitNormal == Vector3::unitY) // Ground
		{
			if (gravityPass) // Don't slide on ground if testing gravity
			{
				computedPos = out_location_secure;
				return true;
			}
		}
		else // Wall
		{
			float direction_dot = Maths::max(0.0f, 1.0f - Vector3::dot(Vector3::normalize(movement), -out_raycast.hitNormal));
			remaining_magnitude *= direction_dot;
		}

		// 9. Project the remaining movement on the collision surface (for sliding)
		Vector3 remaining_movement = Vector3::projectOnPlane(movement, out_raycast.hitNormal);
		remaining_movement.setMagnitude(remaining_magnitude);

		if (remaining_movement == Vector3::zero || Maths::nearZero(remaining_magnitude))
		{
			computedPos = out_location_secure;
			return true; // End the recursion if the movement projected on the collision is null
		}

		// 10. Continue the recursion
		CollideAndSlideAABB(rigidbody, boxAABB, out_location_secure, remaining_movement, bounces + 1, gravityPass, computedPos, colResponses, triggers);
		return true;
	}
	else // No collision encountered, end of the recursion
	{
		computedPos = startPos + movement;
		return false;
	}
}
