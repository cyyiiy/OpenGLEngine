#pragma once
#include <ECS/behaviorComponent.h>


enum class MovementMode : uint8_t
{
	None,
	Wave,
	Circle
};


class BenchmarkComponentMovement : public BehaviorComponent
{
public:
	void initializeTimer(float timer, MovementMode movementMode);

	void update(float deltaTime) override;

private:
	float innerTimer{ 0.0f };
	MovementMode mode{ MovementMode::None };
};

