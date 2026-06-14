#pragma once
#include <ECS/behaviorComponent.h>
#include <Events/observer.h>
#include <Maths/Vector3.h>

class Transform;
class CameraComponent;
class LagMovementComponent;
class BoxCollisionComponent;
class RigidbodyComponent;
class AudioSourceComponent;


/**
* Component to add to an entity to make it a doomlike player.
* It automatically create the needed components on its entity.
*/
class PlayerComponent : public BehaviorComponent, public Observer
{
	const float CAM_SENSITIVITY = 0.12f;
	const float CAM_LAG_SPEED = 8.8f;
	const float CAM_LAG_MAX_DIST = 0.7f;

public:
	void setupPlayer(Entity* camEntity, float camHeight_, float moveSpeed_, float jumpForce_, float stepHeight_);

	void respawn(const Transform& respawnTransform);
	Vector3 getCamPosition() const;

	void onCollision(const BoxCollisionComponent& boxCollided, const Vector3& collisionNormal);

	void init() override;
	void update(float deltaTime) override;

private:
	friend class GunComponent;

	Entity* entity{ nullptr };
	ComponentHandle<CameraComponent> camera;
	ComponentHandle<LagMovementComponent> lagMovement;
	ComponentHandle<BoxCollisionComponent> collision;
	ComponentHandle<RigidbodyComponent> rigidbody;
	ComponentHandle<AudioSourceComponent> feetSoundSource;

	float camHeight{ 0.0f };
	float moveSpeed{ 0.0f };
	float jumpForce{ 0.0f };

	float feetSoundTimer{ 0.0f };
	bool feetSoundAlternance{ false };
	bool onGroundLastFrame{ true };
};


// Specify sublist size for 'PlayerComponent'
template<>
struct ComponentSublistSize<PlayerComponent>
{
	static constexpr size_t value = 1;
};