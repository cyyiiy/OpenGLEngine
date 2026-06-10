#include "rigidbodyComponent.h"
#include "collisionsAABB.h"
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>


void RigidbodyComponent::init()
{}

void RigidbodyComponent::exit()
{
	removeAssociatedCollision();
}

void RigidbodyComponent::update(float dt)
{
	// Kinematic rigidbodies update their movement outside of the physic phase
	if (!isKinematic) return;

	if (!isAssociatedCollisionValid()) return;

	if (firstFrame)
	{
		firstFrame = false;
		return;
	}

	movement = (velocity + velocityOneFrame) * dt;
	gravityMovement = Vector3::zero;
	
	applyMovementsToEntity();
}

void RigidbodyComponent::updatePhysicsPreCollision(float dt)
{
	// Kinematic rigidbodies update their movement outside of the physic phase
	if (isKinematic) return;
	
	if (!isAssociatedCollisionValid()) return;

	if (firstFrame)
	{
		return;
	}

	inPhysicComputation = true;

	// Update gravity velocity and grounded values
	if (useGravity)
	{
		const Vector3& gravity = Locator::getPhysics().GetGravityValue();

		const float gravity_strength = gravity.length();
		const float gravity_velocity_strength = gravityVelocity.length();

		if (gravity_velocity_strength < gravity_strength * 2.0f)
		{
			gravityVelocity += gravity * dt * 2.5f;
		}

		groundedLastFrame = onGround;
		onGround = false;
	}

// Pre-compute the rigidbody movements
movement = (velocity + velocityOneFrame) * dt;
gravityMovement = gravityVelocity * dt;
velocityOneFrame = Vector3::zero;
}

void RigidbodyComponent::updatePhysicsPostCollision(float dt)
{
	// Kinematic rigidbodies update their movement outside of the physic phase
	if (isKinematic) return;

	if (!isAssociatedCollisionValid()) return;

	if (firstFrame)
	{
		firstFrame = false;
		return;
	}

	// Apply inverse step mechanic if possible
	if (useGravity)
	{
		float inverse_step_movement = 0.0f;
		if (checkInverseStepMechanic(inverse_step_movement))
		{
			gravityMovement += Vector3{ 0.0f, inverse_step_movement, 0.0f };
		}
	}

	applyMovementsToEntity();

	inPhysicComputation = false;
}

bool RigidbodyComponent::checkStepMechanic(const BoxCollisionComponent& collidedComp, const Vector3 aimedDestination, const Vector3 hitNormal, float& stepMovement) const
{
	// 1. Continue only if this rigidbody use step mechanic
	if (stepHeight <= 0.0f)
		return false;

	// 2. Continue only if collided with a wall
	if (!(Maths::abs(Vector3::dot(Vector3::unitY, hitNormal)) < 0.5f))
		return false;

	// 3. Continue only if body intersect with collided component at aimed destination
	Box body_box = ECS::GetComponent(associatedCollision).getTransformedBox();
	body_box.setCenterPoint(aimedDestination);
	if (!CollisionsAABB::IntersectAABBRaycast(collidedComp, body_box))
		return false;

	// 4. Compute needed step movement
	Box collided_box = collidedComp.getTransformedBox();
	stepMovement = collided_box.getMaxPoint().y - body_box.getMinPoint().y + Rigidbody::SECURITY_DIST; //  compute needed step movement

	// 5. Continue only if needed step movement is lower than this rigidbody step height
	if (stepMovement > stepHeight)
		return false;

	// 6. Continue only if step destination is free
	body_box.setCenterPoint(aimedDestination + Vector3{ 0.0f, stepMovement, 0.0f });
	if (Locator::getPhysics().AABBRaycast(Vector3::zero, body_box, collisionChannels, 0.0f, true))
		return false;

	return true;
}

void RigidbodyComponent::checkOnGround(const BoxCollisionComponent& groundCollision, const Vector3& impactNormal)
{
	if (!inPhysicComputation)
	{
		Locator::getLog().LogMessage_Category("Rigidbody: Tried to get call 'checkOnGround' outside of the physics phase.", LogCategory::Warning);
		return;
	}

	if (!(impactNormal == Vector3::unitY)) return;

	onGround = true;
	gravityVelocity = Vector3::zero;

	if (ECS::IsComponentHandleValid(groundCollision.getOwningRigidbody()))
	{
		RigidbodyComponent& ground_rigidbody = ECS::GetComponent(groundCollision.getOwningRigidbody());
		if (ground_rigidbody.isKinematic)
		{
			addVelocityOneFrame(ground_rigidbody.velocity);
		}
	}
}


void RigidbodyComponent::associateCollision(ComponentHandle<BoxCollisionComponent> collision)
{
	if (!ECS::IsComponentHandleValid(collision))
	{
		Locator::getLog().LogMessage_Category("Rigidbody: Tried to associate an invalid collision!", LogCategory::Error);
		return;
	}

	if (ECS::GetComponent(collision).isTrigger)
	{
		Locator::getLog().LogMessage_Category("Rigidbody: Tried to associate a trigger collision!", LogCategory::Error);
		return;
	}

	removeAssociatedCollision();

	associatedCollision = collision;
	ECS::GetComponent(associatedCollision).owningRigidbody = getSelfHandle<RigidbodyComponent>();
}

void RigidbodyComponent::removeAssociatedCollision()
{
	if (!isAssociatedCollisionValid()) return;

	ECS::GetComponent(associatedCollision).owningRigidbody = ComponentHandle<RigidbodyComponent>();
	associatedCollision = ComponentHandle<BoxCollisionComponent>();
}

ComponentHandle<BoxCollisionComponent> RigidbodyComponent::getAssociatedCollision() const
{
	return associatedCollision;
}

bool RigidbodyComponent::isAssociatedCollisionValid() const
{
	if (!ECS::IsComponentHandleValid(associatedCollision)) return false;
	
	const BoxCollisionComponent& associated_col_comp = ECS::GetComponent(associatedCollision);
	if (associated_col_comp.isTrigger) return false;

	return true;
}

bool RigidbodyComponent::isOnGround() const
{
	return isAssociatedCollisionValid() && !isKinematic && useGravity && onGround;
}

Vector3 RigidbodyComponent::getTheoricalMovement() const
{
	if (!inPhysicComputation)
	{
		Locator::getLog().LogMessage_Category("Rigidbody: Tried to get theorical movement outside of the physics phase.", LogCategory::Warning);
		return Vector3::zero;
	}

	return movement;
}

Vector3 RigidbodyComponent::getTheoricalGravityMovement() const
{
	if (!inPhysicComputation)
	{
		Locator::getLog().LogMessage_Category("Rigidbody: Tried to get theorical gravity movement outside of the physics phase.", LogCategory::Warning);
		return Vector3::zero;
	}

	return gravityMovement;
}

bool RigidbodyComponent::isFirstFrame() const
{
	return firstFrame;
}

void RigidbodyComponent::addVelocityOneFrame(const Vector3& velocityAdd)
{
	velocityOneFrame += velocityAdd;
}

void RigidbodyComponent::applyComputedMovements(const Vector3& computedMovement, const Vector3& computedGravityMovement)
{
	movement = computedMovement;
	gravityMovement = computedGravityMovement;
}

void RigidbodyComponent::applyMovementsToEntity()
{
	Entity* owner = getOwner();
	if (!owner) return;

	owner->addPosition(movement);
	owner->addPosition(gravityMovement);

	movement = Vector3::zero;
	gravityMovement = Vector3::zero;
}

bool RigidbodyComponent::checkInverseStepMechanic(float& inverseStepMovement) const
{
	if (stepHeight <= 0.0f || gravityMovement.y >= 0.0f || onGround || !groundedLastFrame)
	{
		return false;
	}

	Box box = ECS::GetComponent(associatedCollision).getTransformedBox();
	RaycastHitInfos out;

	bool hit = Locator::getPhysics().AABBSweepRaycast(box.getCenterPoint() + gravityMovement, box.getCenterPoint() + Vector3{ 0.0f, -stepHeight, 0.0f }, box, { "solid" }, out, 0.0f);
	if (hit && out.hitNormal == Vector3::unitY)
	{
		inverseStepMovement = -(out.hitDistance - Rigidbody::SECURITY_DIST);
		return true;
	}

	return false;
}
