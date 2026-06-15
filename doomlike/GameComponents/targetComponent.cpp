#include "targetComponent.h"
#include <ECS/entity.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <ServiceLocator/locator.h>


void TargetComponent::onIntersectedByRaycast(RaycastType type, const Vector3& intersectionPoint)
{
	if (type != RaycastType::RaycastTypeLine) return;
	Locator::getLog().LogMessageToScreen("FPS Demo: Target got hit by a line raycast at coordinates " + intersectionPoint.toString() + ".", Color::yellow, 5.0f);
}

void TargetComponent::init()
{
	if (!getOwner()->hasComponentOfClass<BoxCollisionComponent>())
	{
		Locator::getLog().LogMessage_Category("FPS Demo: A target component was added on an entity that doesn't have a collision component!", LogCategory::Warning);
		return;
	}

	collision = getOwner()->getComponentOfClass<BoxCollisionComponent>();

	ECS::GetComponent(collision).onRaycastIntersect.subscribe(this, &TargetComponent::onIntersectedByRaycast);
}
