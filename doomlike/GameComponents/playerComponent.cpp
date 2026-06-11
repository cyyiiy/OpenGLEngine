#include "playerComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <GameplayStatics/gameplayStatics.h>
#include <Inputs/Input.h>
#include <doomlikeGame.h>

#include <Rendering/cameraComponent.h>
#include <GameComponents/lagMovementComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>
#include <Audio/audioSourceComponent.h>
#include <GameComponents/gunComponent.h>


void PlayerComponent::setupPlayer(Entity* camEntity, float camHeight_, float moveSpeed_, float jumpForce_, float stepHeight_)
{
	lagMovement = camEntity->addComponentByClass<LagMovementComponent>();
	camera = camEntity->addComponentByClass<CameraComponent>();

	camHeight = camHeight_;
	moveSpeed = moveSpeed_;
	jumpForce = jumpForce_;

	CameraComponent& camera_comp = ECS::GetComponent(camera);
	LagMovementComponent& lag_movement_comp = ECS::GetComponent(lagMovement);
	BoxCollisionComponent& collision_comp = ECS::GetComponent(collision);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	AudioSourceComponent& feet_sound_source_comp = ECS::GetComponent(feetSoundSource);

	camera_comp.setOffset(Vector3{ 0.0f, camHeight, 0.0f });
	camera_comp.setAsActiveCamera();

	lag_movement_comp.setupLagMovement(getOwner(), CAM_LAG_SPEED, CAM_LAG_MAX_DIST);

	collision_comp.collisionBox = Box{ Vector3{0.0f, (camHeight / 2.0f) + 0.1f, 0.0f}, Vector3{0.3f, (camHeight / 2.0f) + 0.1f, 0.3f} };
	collision_comp.collisionChannel = "player";
	collision_comp.useEntityScaleForBoxSize = false;

	rigidbody_comp.stepHeight = stepHeight_;
	rigidbody_comp.collisionChannels = { "solid", "enemy", "trigger_zone" };
	rigidbody_comp.onCollisionRepulse.subscribe(this, &PlayerComponent::onCollision);

	feet_sound_source_comp.setSpatialization(ChannelSpatialization::Channel3D);
	feet_sound_source_comp.setOffset(Vector3{ 0.0f, -1.1f, 0.0f });
	feet_sound_source_comp.setVolume(0.2f);

	entity->addGameplayTag("Player");

	setUpdateActivated(true);
}

void PlayerComponent::respawn(const Transform& respawnTransform)
{
	CameraComponent& camera_comp = ECS::GetComponent(camera);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	LagMovementComponent& lag_movement_comp = ECS::GetComponent(lagMovement);

	rigidbody_comp.velocity = Vector3::zero;
	rigidbody_comp.gravityVelocity = Vector3::zero;

	entity->pasteTransform(respawnTransform);

	camera_comp.setPitch(0.0f);
	lag_movement_comp.teleport();

	if (entity->hasComponentOfClass<GunComponent>())
	{
		ECS::GetComponent(entity->getComponentOfClass<GunComponent>()).reset();
	}
}

Vector3 PlayerComponent::getCamPosition() const
{
	return ECS::GetComponent(camera).getCamPosition();
}

void PlayerComponent::onCollision(const BoxCollisionComponent& boxCollided, const Vector3& collisionNormal)
{
	if (collisionNormal == Vector3::negUnitY)
	{
		// Cancel the jump velocity if the player hit a roof
		ECS::GetComponent(rigidbody).gravityVelocity = Vector3::zero;
	}
}


void PlayerComponent::init()
{
	// Create the player components
	entity = getOwner();
	
	collision = entity->addComponentByClass<BoxCollisionComponent>();
	rigidbody = entity->addComponentByClass<RigidbodyComponent>();
	ECS::GetComponent(rigidbody).associateCollision(collision);

	feetSoundSource = entity->addComponentByClass<AudioSourceComponent>();

	setUpdateActivated(false); // Update will be activated once 'setupPlayer' has been called
}

void PlayerComponent::update(float deltaTime)
{
	CameraComponent& camera_comp = ECS::GetComponent(camera);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	AudioSourceComponent& feet_sound_source_comp = ECS::GetComponent(feetSoundSource);

	// Move player
	Vector3 velocity_xz = Vector3::zero;

	if (Input::IsKeyDown(GLFW_KEY_W))
		velocity_xz += entity->getForward() * moveSpeed;

	if (Input::IsKeyDown(GLFW_KEY_S))
		velocity_xz -= entity->getForward() * moveSpeed;

	if (Input::IsKeyDown(GLFW_KEY_A))
		velocity_xz += entity->getRight() * moveSpeed;

	if (Input::IsKeyDown(GLFW_KEY_D))
		velocity_xz -= entity->getRight() * moveSpeed;

	// Clamp the velocity to max movement speed
	velocity_xz.clampMagnitude(moveSpeed);

	// Apply velocity to rigidbody
	rigidbody_comp.velocity = velocity_xz;


	// Player and camera rotation
	Vector2 mouse_delta = Input::GetMouseDelta() * CAM_SENSITIVITY;
	entity->incrementRotation(Quaternion{ Vector3::unitY, -mouse_delta.x * 0.01f });
	camera_comp.setPitch(Maths::clamp(camera_comp.getPitch() + mouse_delta.y, -89.0f, 89.0f));


	// Jump
	if (Input::IsKeyPressed(GLFW_KEY_SPACE) && rigidbody_comp.isOnGround())
	{
		rigidbody_comp.gravityVelocity += (Vector3::unitY * jumpForce);
	}


	// Shoot raycast (debug)
	if (Input::IsKeyPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		Vector3 raycast_start = camera_comp.getCamPosition();
		Vector3 raycast_end = raycast_start + camera_comp.getCamForward() * 5.0f;

		Physics& physics = Locator::getPhysics();
		//physics.LineRaycast(raycast_start, raycast_end, { "solid", "enemy", "trigger_zone" });
		physics.AABBSweepRaycast(raycast_start, raycast_end, Box{ Vector3::zero, Vector3{ 0.1f, 0.1f, 0.1f } }, { "solid", "enemy", "trigger_zone" });
	}


	// Feet sound
	feetSoundTimer -= deltaTime;
	if (rigidbody_comp.isOnGround())
	{
		if (!(velocity_xz == Vector3::zero) || !onGroundLastFrame)
		{
			if (feetSoundTimer <= 0.0f)
			{
				feet_sound_source_comp.playSound(AssetManager::GetSound(feetSoundAlternance ? "feet1" : "feet2"));

				feetSoundAlternance = !feetSoundAlternance;
				feetSoundTimer = 0.5f;
			}
		}
	}

	onGroundLastFrame = rigidbody_comp.isOnGround();


	// Death by void
	if (entity->getPosition().y < -50.0f)
	{
		Locator::getLog().LogMessageToScreen("Doomlike: Player die by falling.", Color::white, 5.0f);
		static_cast<DoomlikeGame*>(GameplayStatics::GetGame())->restartLevel();
	}
}