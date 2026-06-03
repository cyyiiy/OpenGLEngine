#include "benchmarkComponentMovement.h"
#include <ECS/entity.h>


void BenchmarkComponentMovement::initializeTimer(float timer, MovementMode movementMode)
{
	innerTimer = timer;
	mode = movementMode;

	Entity* owner = getOwner();
	const Vector3 owner_pos = owner->getPosition();

	if (mode == MovementMode::Wave)
	{
		const float y_pos = 3.0f + Maths::cos(innerTimer) * 2.0f;
		owner->setPosition(Vector3{ owner_pos.x, y_pos, owner_pos.z });
	}
	if (mode == MovementMode::Circle)
	{
		owner->setPosition(Vector3{ Maths::cos(innerTimer) * 35.0f, owner_pos.y, Maths::sin(innerTimer) * 35.0f });
	}
}

void BenchmarkComponentMovement::update(float deltaTime)
{
	innerTimer += deltaTime * 3.0f;
	
	Entity* owner = getOwner();
	const Vector3 owner_pos = owner->getPosition();

	if (mode == MovementMode::Wave)
	{
		const float y_pos = 3.0f + Maths::cos(innerTimer) * 2.0f;
		owner->setPosition(Vector3{ owner_pos.x, y_pos, owner_pos.z });
	}
	if (mode == MovementMode::Circle)
	{
		owner->setPosition(Vector3{ Maths::cos(innerTimer) * 35.0f, owner_pos.y, Maths::sin(innerTimer) * 35.0f });
	}
}
