#pragma once
#include <ECS/behaviorComponent.h>
#include <Maths/Vector3.h>

class Entity;


/** Lag Movement Component
* A component that moves its entity each frame to follow another entity, with a lag speed.
* Note: Lag only works with position for the moment, not rotation.
*/
class LagMovementComponent : public BehaviorComponent
{
public:
	void setupLagMovement(Entity* targetEntity_, float lagSpeed_, float lagMaxDist_);
	void teleport();

	void init() override;
	void update(float dt) override;

private:
	Entity* targetEntity{ nullptr };
	float lagSpeed{ 0.0f };
	float lagMaxDist{ 0.0f };
};


// Specify sublist size for 'LagMovementComponent'
template<>
struct ComponentSublistSize<LagMovementComponent>
{
	static constexpr size_t value = 2;
};