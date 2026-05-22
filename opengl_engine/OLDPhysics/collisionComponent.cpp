#include "collisionComponent.h"
#include "ObjectChannels/collisionChannels.h"
#include "rigidbodyComponent.h"

#include <ECS/entity.h>

#include <Rendering/material.h>
#include <Rendering/Model/mesh.h>
#include <Utils/color.h>

#include <ServiceLocator/locator.h>



// ----------------------------------------------------------
//  Raycast Tests
// ----------------------------------------------------------
bool CollisionComponent::resolvePoint(const Vector3& point) const
{
	bool intersect = resolvePointIntersection(point);
	return intersect;
}

bool CollisionComponent::resolveLineRaycast(const Ray& raycast, RaycastHitInfos& outHitInfos, const std::vector<std::string> testChannels) const
{
	if (!channelTest(testChannels)) return false;

	bool intersect = resolveLineRaycastIntersection(raycast, outHitInfos);
	//  it's up to the physics manager to broadcast the onRaycastIntersect event
	return intersect;
}

bool CollisionComponent::resolveAABBRaycast(const Box& raycast, const std::vector<std::string> testChannels) const
{
	if (!channelTest(testChannels)) return false;

	bool intersect = resolveAABBRaycastIntersection(raycast);
	//  it's up to the physics manager to broadcast the onRaycastIntersect event
	return intersect;
}

bool CollisionComponent::resolveAABBSweepRaycast(const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, const std::vector<std::string> testChannels, bool forCollisionTest) const
{
	if (!channelTest(testChannels)) return false;

	bool intersect = resolveAABBSweepRaycastIntersection(raycast, boxRaycast, outHitInfos, forCollisionTest);
	//  it's up to the physics manager to broadcast the onRaycastIntersect event
	return intersect;
}


void CollisionComponent::setCollisionType(CollisionType newCollisionType)
{
	collisionType = newCollisionType;
}

void CollisionComponent::setCollisionChannel(const std::string& newCollisionChannel)
{
	collisionChannel = newCollisionChannel;
}

bool CollisionComponent::channelTest(const std::vector<std::string> testChannels) const
{
	for (auto test_channel : testChannels)
	{
		if (test_channel == "") continue;

		if (test_channel == CollisionChannels::DefaultEverything()) return true;

		if (test_channel == collisionChannel) return true;
	}

	return false;
}


// ----------------------------------------------------------
//  Collision getters
// ----------------------------------------------------------
CollisionShape CollisionComponent::getCollisionShape() const
{
	return collisionShape;
}

CollisionType CollisionComponent::getCollisionType() const
{
	return collisionType;
}

std::string CollisionComponent::getCollisionChannel() const
{
	return collisionChannel;
}

const Matrix4 CollisionComponent::getModelMatrix() const
{
	return getOwner()->getModelMatrix(); //  this function is made to be overriden
}

Vector3 CollisionComponent::getCenterDownPos() const
{
	return Vector3::zero; //  this function is made to be overriden
}

Box CollisionComponent::getEncapsulatingBox() const
{
	return Box::zero; //  this function is made to be overriden
}


// ----------------------------------------------------------
//  Rigidbody
// ----------------------------------------------------------
void CollisionComponent::setOwningRigidbody(RigidbodyComponent* rigidbodyOwner)
{
	owningRigidbody = rigidbodyOwner;
}

RigidbodyComponent* CollisionComponent::getOwningRigidbody() const
{
	return owningRigidbody;
}

bool CollisionComponent::ownedByRigidbody() const
{
	return owningRigidbody != nullptr;
}


// ----------------------------------------------------------
//  Debug Drawing
// ----------------------------------------------------------
void CollisionComponent::drawDebug(Material& debugMaterial)
{
	debugMaterial.getShader().setVec3("color", debugIntersectedLastFrame ? Color::red : Color::green);
	drawDebugMesh(debugMaterial);
}

void CollisionComponent::setDebugIntersected(bool debugIntersected) const
{
	debugIntersectedLastFrame = debugIntersected;
}

bool CollisionComponent::getDebugIntersected() const
{
	return debugIntersectedLastFrame;
}



// ----------------------------------------------------------
//  Component registering functions
// ----------------------------------------------------------
void CollisionComponent::registerComponent()
{
	Locator::getPhysics().RegisterCollision(this);
}

void CollisionComponent::unregisterComponent()
{
	Locator::getPhysics().UnregisterCollision(this);
}

void CollisionComponent::exit()
{
	if (isAudioCollision)
	{
		Locator::getAudio().ReleaseCollision(audioCollisionIndex);
	}

	onCollisionDelete.broadcast();
}

void CollisionComponent::resetValues()
{
	//  reset the values in case this component was used before (the component manager is a memory pool) | called in init on derived classes
	//  note: no need to reset collision shape and debug mesh, they are set in the init of the derived classes
	collisionType = CollisionType::Solid;
	isAudioCollision = false;
	audioCollisionIndex = 0;
	collisionChannel = "";
	debugIntersectedLastFrame = false;
	owningRigidbody = nullptr;
}
