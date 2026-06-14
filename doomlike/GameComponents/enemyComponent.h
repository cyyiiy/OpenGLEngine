#pragma once
#include <ECS/behaviorComponent.h>
#include <Events/observer.h>
#include <Events/event.h>
#include <Maths/Vector3.h>

class ModelRendererComponent;
class BoxCollisionComponent;
class RigidbodyComponent;
class PlayerComponent;


/**
* Component to add to an entity to make it a doomlike enemy.
* It automatically creates the needed components on its entity.
*/
class EnemyComponent : public BehaviorComponent, public Observer
{
	const float DETECTION_RANGE = 9.0f;
	const float MOVE_SPEED = 2.0f;

public:
	Event<Entity*> onDie;

	void init() override;
	void update(float dt) override;

	void onBodyIntersection(const RigidbodyComponent& body, const Vector3& hitNormal);

private:
	Entity* entity{ nullptr };
	ComponentHandle<ModelRendererComponent> enemyModel;
	ComponentHandle<BoxCollisionComponent> collision;
	ComponentHandle<RigidbodyComponent> rigidbody;

	ComponentHandle<PlayerComponent> playerRef;
	bool dead{ false };
};


// Specify sublist size for 'EnemyComponent'
template<>
struct ComponentSublistSize<EnemyComponent>
{
	static constexpr size_t value = 4;
};