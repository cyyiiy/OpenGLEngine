#include "collisionTests.h"
#include <ServiceLocator/locator.h>
#include <Physics/rigidbodyComponent.h>
#include <Physics/AABB/boxAABBColComp.h>
#include <Maths/Geometry/box.h>


bool CollisionTests::RigidbodyCollideAndSlideAABB(const RigidbodyComponent& rigidbody, const bool gravityPass, Vector3& computedMovement, std::vector<CollisionHit>& colResponses, std::vector<const CollisionComponent*>& triggers)
{
	const Vector3 body_start_movement = gravityPass ? rigidbody.getAnticipatedGravityMovement() : rigidbody.getAnticipatedMovement();
	if (body_start_movement == Vector3::zero)
	{
		computedMovement = body_start_movement;
		return false; //  Do not check collisions if no movement
	}

	const CollisionComponent* col_comp = &rigidbody.getAssociatedCollision();
	Box body_shape = static_cast<const BoxAABBColComp*>(col_comp)->getTransformedBox();
	const Vector3 body_start_pos = body_shape.getCenterPoint();
	body_shape.setCenterPoint(Vector3::zero);

	Vector3 computed_pos = Vector3::zero;

	bool col_hit = CollideAndSlideAABB(rigidbody, col_comp, body_shape, body_start_pos, body_start_movement, 0, gravityPass, computed_pos, colResponses, triggers);
	computedMovement = computed_pos - body_start_pos;

	return col_hit;
}


bool CollisionTests::CollideAndSlideAABB(const RigidbodyComponent& rigidbody, const CollisionComponent* colComp, const Box& boxAABB, const Vector3 startPos, const Vector3 movement, const int bounces, const bool gravityPass, Vector3& computedPos, std::vector<CollisionHit>& colResponses, std::vector<const CollisionComponent*>& triggers)
{
	RaycastHitInfos out_raycast;
	bool col_encountered = Locator::getPhysics().AABBSweepPhysicTest(startPos, startPos + movement, boxAABB, rigidbody.getTestChannels(), colComp, out_raycast);
	
	//  check for triggers
	if (!out_raycast.triggersDetected.empty())
	{
		for (auto& trigger : out_raycast.triggersDetected)
		{
			auto iter = std::find(triggers.begin(), triggers.end(), trigger);
			if (iter == triggers.end())
				triggers.push_back(trigger);
		}
	}

	if (col_encountered) //  collision encountered, continuing recursion
	{
		float step_mechanic = 0.0f;
		if (rigidbody.checkStepMechanic(*out_raycast.hitCollision, startPos + movement, out_raycast.hitNormal, step_mechanic))
		{
			computedPos = startPos + movement + Vector3{ 0.0f, step_mechanic, 0.0f };

			colResponses.push_back(CollisionHit{
				*out_raycast.hitCollision,
				computedPos + Vector3::negUnitY * boxAABB.getHalfExtents(),
				Vector3::unitY
				});

			return true;
		}

		colResponses.push_back(CollisionHit{ 
			*out_raycast.hitCollision, 
			out_raycast.hitLocation + (-out_raycast.hitNormal * boxAABB.getHalfExtents()), 
			out_raycast.hitNormal});

		const Vector3 out_location_secure = out_raycast.hitLocation += -movement * Rigidbody::SECURITY_DIST;

		if (bounces > Rigidbody::MAX_BOUNCES) //  stop recursion if max bounces is reached
		{
			computedPos = out_location_secure;
			return true;
		}

		float remaining_magnitude = movement.length() - out_raycast.hitDistance;

		float col_flatness = Vector3::dot(Vector3::unitY, out_raycast.hitNormal); //  1 = perfectly flat
		if (col_flatness > 0.5f) //  ground or walkable slope
		{
			if (gravityPass) //  doesn't slide if testing for gravity
			{
				computedPos = out_location_secure;
				return true;
			}
		}
		else //  wall or steep slope
		{
			float direction_dot = Maths::max(0.0f, 1.0f - Vector3::dot(Vector3::normalize(movement), -out_raycast.hitNormal));
			remaining_magnitude *= direction_dot;
		}

		Vector3 remaining_movement = Vector3::projectOnPlane(movement, out_raycast.hitNormal);
		remaining_movement.setMagnitude(remaining_magnitude);

		if (remaining_movement == Vector3::zero || Maths::nearZero(remaining_magnitude)) //  if the remaining movement projected on plane is null, stop recursion
		{
			computedPos = out_location_secure;
			return true;
		}


		CollideAndSlideAABB(rigidbody, colComp, boxAABB, out_location_secure, remaining_movement, bounces + 1, gravityPass, computedPos, colResponses, triggers);
		return true;
	}
	else //  no collision encountered, end of the recursion
	{
		computedPos = startPos + movement;
		return false;
	}
}
