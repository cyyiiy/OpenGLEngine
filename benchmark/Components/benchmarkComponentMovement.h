#pragma once
#include <ECS/component.h>


enum class MovementMode : uint8_t
{
	None,
	Wave,
	Circle
};


class BenchmarkComponentMovement : public Component
{
public:
	void initializeTimer(float timer, MovementMode movementMode);

protected:
	void update(float deltaTime) override;

private:
	float innerTimer{ 0.0f };
	MovementMode mode{ MovementMode::None };
};

