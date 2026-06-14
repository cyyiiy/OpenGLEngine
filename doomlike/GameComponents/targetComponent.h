#pragma once
#include <ECS/BehaviorComponent.h>
#include <Events/observer.h>
#include <PhysicsAABB/raycastUtils.h>

class BoxCollisionComponent;


/**
* Components that print a message if its owner is hit by a line raycast.
* This component need its owner to already have a collision component.
*/
class TargetComponent : public BehaviorComponent, public Observer
{
public:
	void onIntersectedByRaycast(RaycastType type, const Vector3& intersectionPoint);

	void init() override;

private:
	ComponentHandle<BoxCollisionComponent> collision;
};


// Specify sublist size for 'TargetComponent'
template<>
struct ComponentSublistSize<TargetComponent>
{
	static constexpr size_t value = 4;
};