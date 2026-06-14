#include "lagMovementComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>


void LagMovementComponent::setupLagMovement(Entity* targetEntity_, float lagSpeed_, float lagMaxDist_)
{
	targetEntity = targetEntity_;
	lagSpeed = lagSpeed_;
	lagMaxDist = lagMaxDist_;

	teleport();

	setUpdateActivated(true);
}

void LagMovementComponent::teleport()
{
	if (!targetEntity) return;

	Entity* lag_entity = getOwner();
	if (lag_entity == nullptr) return;

	lag_entity->pasteTransform(*targetEntity);
}

void LagMovementComponent::init()
{
	if (getOwner() == nullptr)
	{
		Locator::getLog().LogMessage_Category("Lag Movement Component: A lag movemement component was created without an owner entity!", LogCategory::Error);
	}

	setUpdateActivated(false); // Update will be activated once 'setupLagMovement' will be called
}

void LagMovementComponent::update(float dt)
{
	if (!targetEntity) return;

	Entity* lag_entity = getOwner();
	if (lag_entity == nullptr) return;


	// Compute the lag position
	const Vector3 target_pos = targetEntity->getPosition();
	Vector3 lag_pos = lag_entity->getPosition();

	float lag_dist = Vector3::Distance(lag_pos, target_pos);

	if (lag_dist > lagMaxDist)
	{
		const Vector3 inv_lag_dir = Vector3::normalize(target_pos - lag_pos);
		lag_pos = target_pos - inv_lag_dir * lagMaxDist;
		lag_dist = lagMaxDist;
	}

	float lag_alpha = Maths::min(lagSpeed * dt / lag_dist, 1.0f);
	lag_pos = Vector3::lerp(lag_pos, target_pos, lag_alpha);


	// Apply the lag position
	lag_entity->pasteTransform(*targetEntity);
	lag_entity->setPosition(lag_pos);
}
