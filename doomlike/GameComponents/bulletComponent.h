#pragma once
#include <ECS/behaviorComponent.h>
#include <Events/observer.h>
#include <Maths/Vector3.h>
#include <Maths/Quaternion.h>

class ModelRendererComponent;
class BoxCollisionComponent;
class RigidbodyComponent;


/**
* Component to add to an entity to make it a bullet.
* It automatically create the needed components on its entity.
*/
class BulletComponent : public BehaviorComponent, public Observer
{
public:
	void setupBullet(const Vector3& spawnPos, const Quaternion& spawnRot, const Vector3& bulletDirection, const float bulletVelocity, const float bulletLifetime);
	void deleteBullet();

	bool isLifetimeOver() const;

	void onBulletCollisionHit(const BoxCollisionComponent& boxCollided, const Vector3& collisionNormal);
	void onBulletRigidbodyHit(const RigidbodyComponent& bodyCollided, const Vector3& collisionNormal);

	virtual void init() override;
	virtual void update(float deltaTime) override;

private:
	ComponentHandle<ModelRendererComponent> bulletModel;
	ComponentHandle<BoxCollisionComponent> collision;
	ComponentHandle<RigidbodyComponent> rigidbody;

	float lifetime{ 0.0f };
};


// Specify sublist size for 'BulletComponent'
template<>
struct ComponentSublistSize<BulletComponent>
{
	static constexpr size_t value = 16;
};