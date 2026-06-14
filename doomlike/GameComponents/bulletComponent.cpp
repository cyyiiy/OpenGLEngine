#include "bulletComponent.h"
#include <ECS/entity.h>
#include <Assets/assetManager.h>

#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>


void BulletComponent::setupBullet(const Vector3& spawnPos, const Quaternion& spawnRot, const Vector3& bulletDirection, const float bulletVelocity, const float bulletLifetime)
{
	getOwner()->setPosition(spawnPos);
	getOwner()->setRotation(spawnRot);
	lifetime = bulletLifetime;

	ModelRendererComponent& bullet_model_comp = ECS::GetComponent(bulletModel);
	BoxCollisionComponent& collision_comp = ECS::GetComponent(collision);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);

	bullet_model_comp.model = &AssetManager::GetModel("bullet");
	bullet_model_comp.offset.setRotation(Quaternion{ Vector3::unitY, Maths::toRadians(90.0f) });
	bullet_model_comp.offset.setScale(0.0002f);

	collision_comp.collisionBox = Box{ Vector3::zero, Vector3{ 0.05f, 0.05f, 0.05f } };
	collision_comp.collisionChannel = "bullet";
	collision_comp.useEntityScaleForBoxSize = false;

	rigidbody_comp.collisionChannels = { "solid", "enemy", "trigger_zone" };
	rigidbody_comp.useGravity = false;
	rigidbody_comp.velocity = bulletDirection * bulletVelocity;

	rigidbody_comp.onCollisionRepulse.subscribe(this, &BulletComponent::onBulletCollisionHit);
	collision_comp.onCollisionIntersect.subscribe(this, &BulletComponent::onBulletRigidbodyHit);

	getOwner()->addGameplayTag("Bullet");

	setUpdateActivated(true);
}

void BulletComponent::deleteBullet()
{
	ECS::GetComponent(rigidbody).onCollisionRepulse.unsubscribe(this);
	ECS::GetComponent(collision).onCollisionIntersect.unsubscribe(this);

	getOwner()->destroyEntity();
}

bool BulletComponent::isLifetimeOver() const
{
	return lifetime <= 0.0f;
}

void BulletComponent::onBulletCollisionHit(const BoxCollisionComponent& boxCollided, const Vector3& collisionNormal)
{
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	rigidbody_comp.onCollisionRepulse.unsubscribe(this);

	rigidbody_comp.velocity = Vector3::zero;
	rigidbody_comp.useGravity = true;
}

void BulletComponent::onBulletRigidbodyHit(const RigidbodyComponent& bodyCollided, const Vector3& collisionNormal)
{
	if (bodyCollided.getOwner()->hasGameplayTag("Enemy"))
	{
		lifetime = 0.0f; // Allow the gun component to properly delete the bullet
	}
}

void BulletComponent::init()
{
	// Create the bullet components
	bulletModel = getOwner()->addComponentByClass<ModelRendererComponent>();
	collision = getOwner()->addComponentByClass<BoxCollisionComponent>();
	rigidbody = getOwner()->addComponentByClass<RigidbodyComponent>();
	ECS::GetComponent(rigidbody).associateCollision(collision);

	setUpdateActivated(false); // Update will be activated once 'setupBullet' has been called
}

void BulletComponent::update(float deltaTime)
{
	lifetime -= deltaTime;
}