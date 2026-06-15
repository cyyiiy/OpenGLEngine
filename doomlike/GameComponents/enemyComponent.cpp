#include "enemyComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <GameplayStatics/gameplayStatics.h>
#include <doomlikeGame.h>

#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>
#include <GameComponents/playerComponent.h>


void EnemyComponent::init()
{
	// Create the enemy components
	entity = getOwner();

	enemyModel = entity->addComponentByClass<ModelRendererComponent>();
	collision = entity->addComponentByClass<BoxCollisionComponent>();
	rigidbody = entity->addComponentByClass<RigidbodyComponent>();

	ModelRendererComponent& enemy_model_comp = ECS::GetComponent(enemyModel);
	BoxCollisionComponent& collision_comp = ECS::GetComponent(collision);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);

	enemy_model_comp.model = &AssetManager::GetModel("enemy");
	enemy_model_comp.offset.setScale(0.7f);

	collision_comp.collisionBox = Box{ Vector3::zero, Vector3{ 0.5f, 0.5f, 0.5f } };
	collision_comp.collisionChannel = "enemy";
	collision_comp.onCollisionIntersect.subscribe(this, &EnemyComponent::onBodyIntersection);

	rigidbody_comp.associateCollision(collision);
	rigidbody_comp.collisionChannels = { "solid", "player", "bullet" };
	rigidbody_comp.useGravity = false;

	playerRef = static_cast<DoomlikeGame*>(GameplayStatics::GetGame())->getPlayer();
	if (!ECS::IsComponentHandleValid(playerRef))
	{
		Locator::getLog().LogMessage_Category("FPS Demo: An enemy couldn't retrieve the reference of the player!", LogCategory::Error);
		setUpdateActivated(false);
	}

	entity->addGameplayTag("Enemy");
}

void EnemyComponent::update(float deltaTime)
{
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	PlayerComponent& player_ref_comp = ECS::GetComponent(playerRef);

	rigidbody_comp.velocity = Vector3::zero;
	const Vector3 player_cam_pos = player_ref_comp.getCamPosition();

	RaycastHitInfos out;
	bool test_player = Locator::getPhysics().LineRaycast(entity->getPosition(), player_cam_pos, { "solid", "player" }, out, 0.0f);
	if (!test_player) return;

	if (out.hitDistance > DETECTION_RANGE) return;
	if (!ECS::GetComponent(out.hitCollision).getOwner()->hasGameplayTag("Player")) return;

	entity->rotateTowards(player_cam_pos);

	rigidbody_comp.velocity = Vector3::normalize(player_cam_pos - entity->getPosition()) * MOVE_SPEED;
}

void EnemyComponent::onBodyIntersection(const RigidbodyComponent& body, const Vector3& hitNormal)
{
	if (dead) return;

	if (body.getOwner()->hasGameplayTag("Bullet"))
	{
		Locator::getLog().LogMessageToScreen("FPS Demo: Enemy die from a bullet.", Color::white, 5.0f);
		dead = true;

		onDie.broadcast(entity);

		// Play death sound
		Locator::getAudio().InstantPlaySound3D(AssetManager::GetSound("enemydeath"), entity->getPosition(), 0.15f);

		entity->destroyEntity();
	}
	else if (body.getOwner()->hasGameplayTag("Player"))
	{
		Locator::getLog().LogMessageToScreen("FPS Demo: Player die from the enemy.", Color::white, 5.0f);
		dead = true; // It allows to avoid this being called twice

		// Play player death sound
		Locator::getAudio().InstantPlaySound2D(AssetManager::GetSound("playerdeath"), 0.4f);

		static_cast<DoomlikeGame*>(GameplayStatics::GetGame())->restartLevel();
	}
}