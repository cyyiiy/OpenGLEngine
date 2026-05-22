#include "rigidbodyComponent.h"
#include "ObjectChannels/collisionChannels.h"

#include <ECS/entity.h>

#include <ServiceLocator/locator.h>



// ----------------------------------------------------------
//  Associated Collision
// ----------------------------------------------------------
void RigidbodyComponent::associateCollision(std::weak_ptr<CollisionComponent> collisionToAssociate)
{
	//  remove the owning rigidbody of a potential previous managed collision
	if (isAssociatedCollisionValid())
	{
		associatedCollision->onCollisionIntersect.unregisterObserver(this);
		associatedCollision->setOwningRigidbody(nullptr);
	}

	associatedCollision = collisionToAssociate.lock();
	if (associatedCollision)
	{
		//  initialize the collision as a managed collision
		associatedCollision->setOwningRigidbody(this);
		associatedCollision->onCollisionIntersect.registerObserver(this, Bind_2(&RigidbodyComponent::onCollisionIntersected));
	}
}

bool RigidbodyComponent::isAssociatedCollisionValid() const
{
	return associatedCollision.operator bool();
}

const CollisionComponent& RigidbodyComponent::getAssociatedCollision() const
{
	return *associatedCollision;
}

CollisionComponent& RigidbodyComponent::getAssociatedCollisionNonConst()
{
	return *associatedCollision;
}


// ----------------------------------------------------------
//  Rigidbody Parameters
// ----------------------------------------------------------
void RigidbodyComponent::setPhysicsActivated(bool value)
{
	physicsActivated = value;
}

void RigidbodyComponent::setUseGravity(bool value)
{
	useGravity = value;
}

void RigidbodyComponent::setStepHeight(float value)
{
	stepHeight = value;
}

bool RigidbodyComponent::isPhysicsActivated() const
{
	return isAssociatedCollisionValid() && physicsActivated;
}

bool RigidbodyComponent::getUseGravity() const
{
	return useGravity;
}

float RigidbodyComponent::getStepHeight() const
{
	return stepHeight;
}


// ----------------------------------------------------------
//  Velocity
// ----------------------------------------------------------
void RigidbodyComponent::setVelocity(const Vector3& value)
{
	velocity = value;
}

void RigidbodyComponent::addVelocity(const Vector3& value)
{
	velocity += value;
}

void RigidbodyComponent::addVelocityOneFrame(const Vector3& value)
{
	velocityOneFrame += value;
}

Vector3 RigidbodyComponent::getVelocity() const
{
	return velocity;
}

Vector3 RigidbodyComponent::getAnticipatedMovement() const
{
	return movement;
}


// ----------------------------------------------------------
//  Gravity Velocity
// ----------------------------------------------------------
void RigidbodyComponent::setGravityVelocity(const Vector3& value)
{
	gravityVelocity = value;
}

void RigidbodyComponent::addGravityVelocity(const Vector3& value)
{
	gravityVelocity += value;
}

Vector3 RigidbodyComponent::getGravityVelocity() const
{
	return gravityVelocity;
}

Vector3 RigidbodyComponent::getAnticipatedGravityMovement() const
{
	return gravityMovement;
}


// ----------------------------------------------------------
//  Other Rigidbody functions
// ----------------------------------------------------------
bool RigidbodyComponent::checkStepMechanic(const CollisionComponent& collidedComp, const Vector3 aimedDestination, const Vector3 hitNormal, float& stepMovement) const
{
	if (stepHeight <= 0.0f)
		return false; //  continue only if this rigidbody use step mechanic

	if (!(Maths::abs(Vector3::dot(Vector3::unitY, hitNormal)) < 0.5f))
		return false; //  continue only if collided with a wall

	Box body_box = associatedCollision->getEncapsulatingBox();
	body_box.setCenterPoint(aimedDestination);
	if (!collidedComp.resolveAABBRaycast(body_box, getTestChannels()))
		return false; //  continue only if body intersect with collided at aimed destination

	Box collided_box = collidedComp.getEncapsulatingBox();
	stepMovement = collided_box.getMaxPoint().y - body_box.getMinPoint().y + Rigidbody::SECURITY_DIST; //  compute needed step movement

	if (stepMovement > stepHeight)
		return false; //  continue only if needed step movement is lower than this rigidbody step height

	body_box.setCenterPoint(aimedDestination + Vector3{ 0.0f, stepMovement, 0.0f });
	if (Locator::getPhysics().AABBRaycast(Vector3::zero, body_box, getTestChannels(), 0.0f, true))
		return false; //  continue only if step destination is free

	return true;
}

bool RigidbodyComponent::isOnGround() const
{
	return useGravity && onGround;
}


//  Observer functions
// -------------------------
void RigidbodyComponent::onCollisionIntersected(RigidbodyComponent& other, const CollisionResponse& collisionResponse)
{
	if (isPhysicsActivated() || !other.isPhysicsActivated()) return;
	//  continue only if this rigidbody isn't physics activated and the other is physic activated

	float col_top = Vector3::dot(Vector3::unitY, collisionResponse.impactNormal);
	if (col_top < 0.5f) return;
	//  continue only if the other body is on top of this body

	other.addVelocityOneFrame(getVelocity());
}

void RigidbodyComponent::onCollision(const CollisionResponse& collisionResponse)
{
	if (collisionResponse.impactNormal == Vector3::unitY)
	{
		onGround = true;
		gravityVelocity = Vector3::zero;
	}
	//  might change that later to allow onGround even on non perfectly flat surfaces using dot product
	//  not necessary now since there is only AABB currently implemented in this engine
}


// ----------------------------------------------------------
//  Rigibody Test Channels
// ----------------------------------------------------------
void RigidbodyComponent::setTestChannels(std::vector<std::string> newTestChannels)
{
	testChannels = newTestChannels;
}

void RigidbodyComponent::addTestChannel(std::string newTestChannel)
{
	testChannels.push_back(newTestChannel);
}

std::vector<std::string> RigidbodyComponent::getTestChannels() const
{
	if (testChannels.empty()) return CollisionChannels::GetRegisteredTestChannel("TestEverything");

	return testChannels;
}



// ----------------------------------------------------------
//  Component registering functions
// ----------------------------------------------------------
void RigidbodyComponent::registerComponent()
{
	Locator::getPhysics().RegisterRigidbody(this);
}

void RigidbodyComponent::unregisterComponent()
{
	Locator::getPhysics().UnregisterRigidbody(this);
}

void RigidbodyComponent::init()
{
	//  reset the values in case this component was used before (the component manager is a memory pool)
	physicsActivated = false;
	useGravity = false;
	stepHeight = 0.0f;
	velocity = Vector3::zero;
	velocityOneFrame = Vector3::zero;
	movement = Vector3::zero;
	gravityVelocity = Vector3::zero;
	gravityMovement = Vector3::zero;
	onGround = false;
	groundedLastFrame = false;
	firstFrame = true;
	testChannels.clear();

	onCollisionRepulsed.registerObserver(this, Bind_1(&RigidbodyComponent::onCollision));
}

void RigidbodyComponent::exit()
{
	onCollisionRepulsed.unregisterObserver(this);

	if (isAssociatedCollisionValid())
	{
		associatedCollision->onCollisionIntersect.unregisterObserver(this);
		associatedCollision->setOwningRigidbody(nullptr);
	}


	//  released shared pointer
	associatedCollision = nullptr;
}



// ----------------------------------------------------------
//  Update Physics functions for the Physics Manager
// ----------------------------------------------------------
void RigidbodyComponent::updatePhysicsPreCollision(float dt)
{
	if (firstFrame) return;

	if (!isAssociatedCollisionValid()) return;
	
	//  compute gravity in velocity
	if (useGravity)
	{
		float gravity_strength = Locator::getPhysics().GetGravityValue();

		if (gravityVelocity.y > gravity_strength * 2.0f)
			gravityVelocity.y += gravity_strength * dt * 2.5f;


		groundedLastFrame = onGround;
		onGround = false;
	}

	//  compute anticipated movement
	if (velocityOneFrame.y != 0.0f)
	{
		//  y velocity one frame will make the rigidbody start the test inside the collision that gave it
		associatedCollision->getOwner()->addPosition(Vector3{0.0f, velocityOneFrame.y, 0.0f} *dt);
		velocityOneFrame.y = 0.0f;
	}

	movement = (velocity + velocityOneFrame) * dt;
	gravityMovement = gravityVelocity * dt;
	velocityOneFrame = Vector3::zero;

	//  if rigidbody has no physic activated, it moves but without checking for collisions to repulse its movement
	if (!isPhysicsActivated())
	{
		associatedCollision->getOwner()->addPosition(movement);
		associatedCollision->getOwner()->addPosition(gravityMovement);
		movement = Vector3::zero;
		gravityMovement = Vector3::zero;
	}
}

void RigidbodyComponent::updatePhysicsPostCollision(float dt)
{
	if (firstFrame)
	{
		firstFrame = false;
		return;
	}

	if (!isAssociatedCollisionValid()) return;

	if (!isPhysicsActivated()) return;


	//  inversed step mechanic
	if (getStepHeight() > 0.0f && groundedLastFrame && !isOnGround() && gravityMovement.y < 0.0f)
	{
		Box box = associatedCollision->getEncapsulatingBox();
		RaycastHitInfos out;

		bool hit = Locator::getPhysics().AABBSweepRaycast(box.getCenterPoint() + gravityMovement, box.getCenterPoint() + Vector3{ 0.0f, -stepHeight, 0.0f }, box, { "solid" }, out, 0.0f);
		if (hit)
		{
			if (out.hitNormal == Vector3::unitY)
			{
				gravityMovement += Vector3{ 0.0f, -(out.hitDistance - Rigidbody::SECURITY_DIST), 0.0f };
			}
		}
	}


	//  apply real movement (anticipated movement modified by the collisions during physics step)
	associatedCollision->getOwner()->addPosition(movement);
	associatedCollision->getOwner()->addPosition(gravityMovement);
	movement = Vector3::zero;
	gravityMovement = Vector3::zero;
}


void RigidbodyComponent::applyComputedMovement(const Vector3& computedMovement)
{
	if (!isPhysicsActivated()) return;

	movement = computedMovement;
}

void RigidbodyComponent::applyComputedGravityMovement(const Vector3& computedGravityMovement)
{
	if (!isPhysicsActivated()) return;

	gravityMovement = computedGravityMovement;
}