#include "movingPlatformComponent.h"
#include <ECS/entity.h>
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>

#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>
#include <Audio/audioSourceComponent.h>


void MovingPlatformComponent::setupMovingPlatform(const Vector3& pointA_, const Vector3& pointB_, const float movementSpeed_, const float waitTime_, const bool debug_)
{
	pointA = pointA_;
	pointB = pointB_;
	timeAtoB = Vector3::Distance(pointA, pointB) / movementSpeed_;
	waitTime = waitTime_;
	timer = 0.0f;
	waitTimer = 0.0f;
	reverse = false;

	debug = debug_;

	BoxCollisionComponent& collision_comp = ECS::GetComponent(collision);
	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	AudioSourceComponent& audio_source_comp = ECS::GetComponent(audioSource);

	getOwner()->setPosition(pointA);
	rigidbody_comp.velocity = (pointB - pointA) * (1.0f / timeAtoB);

	collision_comp.collisionBox = Box::one;
	collision_comp.collisionChannel = "solid";

	rigidbody_comp.isKinematic = true;
	rigidbody_comp.useGravity = false;

	audio_source_comp.setSpatialization(ChannelSpatialization::Channel3D);
	audio_source_comp.playSound(AssetManager::GetSound("elevator"), -1);

	setUpdateActivated(true);

	if (debug)
	{
		Locator::getLog().LogMessageToScreen(
			"Moving Platform Setup: {Point A: " + pointA.toString() + "}  {Point B: " + pointB.toString() + "}  {Time A to B: " + std::to_string(timeAtoB) + "}  {Start Position: " + getOwner()->getPosition().toString() + "}", 
			Color::cyan, 99999999.0f, "moving_platform_debug_setup");
	}
}

void MovingPlatformComponent::pauseMovement()
{
	paused = true;
	ECS::GetComponent(rigidbody).velocity = Vector3::zero;
	ECS::GetComponent(audioSource).setPause(true);
}

void MovingPlatformComponent::resumeMovement()
{
	paused = false;
	if (waiting) return;

	ECS::GetComponent(rigidbody).velocity = (reverse ? (pointA - pointB) : (pointB - pointA)) * (1.0f / timeAtoB);
	ECS::GetComponent(audioSource).setPause(false);
}

void MovingPlatformComponent::init()
{
	// Create the moving platform components
	collision = getOwner()->addComponentByClass<BoxCollisionComponent>();
	rigidbody = getOwner()->addComponentByClass<RigidbodyComponent>();
	ECS::GetComponent(rigidbody).associateCollision(collision);
	audioSource = getOwner()->addComponentByClass<AudioSourceComponent>();

	setUpdateActivated(false); // Update will be activatedonce 'setupMovingPlatform' has been called
}

void MovingPlatformComponent::update(float dt)
{
	if (debug)
	{
		updateDebug(dt);
	}

	if (paused) return;

	RigidbodyComponent& rigidbody_comp = ECS::GetComponent(rigidbody);
	AudioSourceComponent& audio_source_comp = ECS::GetComponent(audioSource);

	if (waiting)
	{
		waitTimer -= dt;
		if (waitTimer <= 0.0f)
		{
			waitTimer = 0.0f;
			waiting = false;

			resumeMovement();
		}
		return;
	}

	if (reverse)
	{
		timer -= dt;
		if (timer <= 0.0f) // Reached point A
		{
			getOwner()->setPosition(pointA);
			timer = 0.0f;
			reverse = false;
			rigidbody_comp.velocity = (pointB - pointA) * (1.0f / timeAtoB);

			if (waitTime > 0.0f)
			{
				waitTimer = waitTime;
				waiting = true;
				rigidbody_comp.velocity = Vector3::zero;

				audio_source_comp.setPause(true);
			}
		}
	}
	else
	{
		timer += dt;
		if (timer >= timeAtoB) // Reached point B
		{
			getOwner()->setPosition(pointB);
			timer = timeAtoB;
			reverse = true;
			rigidbody_comp.velocity = (pointA - pointB) * (1.0f / timeAtoB);
			if (waitTime > 0.0f)
			{
				waitTimer = waitTime;
				waiting = true;
				rigidbody_comp.velocity = Vector3::zero;

				audio_source_comp.setPause(true);
			}
		}
	}
}

void MovingPlatformComponent::updateDebug(float dt)
{
	Locator::getLog().LogMessageToScreen("-- Moving Platform Debug New Frame --  {Delta Time: " + std::to_string(dt) + "}", Color::white, 5.0f, "moving_platform_debug_deltatime");

	if (paused)
	{
		Locator::getLog().LogMessageToScreen("Moving Platform State: Paused", Color::magenta, 5.0f, "moving_platform_debug_state");
		Locator::getLog().LogMessageToScreen("Moving Platform Position: " + getOwner()->getPosition().toString(), Color::yellow, 5.0f, "moving_platform_debug_position");
		Locator::getLog().EraseLogIndexMessage("moving_platform_debug_timer");
		Locator::getLog().EraseLogIndexMessage("moving_platform_debug_velocity");
		return;
	}

	if (waiting)
	{
		Locator::getLog().LogMessageToScreen("Moving Platform State: Waiting", Color::magenta, 5.0f, "moving_platform_debug_state");
		Locator::getLog().LogMessageToScreen("Moving Platform Position: " + getOwner()->getPosition().toString(), Color::yellow, 5.0f, "moving_platform_debug_position");
		Locator::getLog().LogMessageToScreen("Moving Platform Wait Timer: " + std::to_string(waitTimer), Color::yellow, 5.0f, "moving_platform_debug_timer");
		Locator::getLog().EraseLogIndexMessage("moving_platform_debug_velocity");
		return;
	}

	if (reverse)
	{
		Locator::getLog().LogMessageToScreen("Moving Platform State: Moving reverse", Color::magenta, 5.0f, "moving_platform_debug_state");

	}
	else
	{
		Locator::getLog().LogMessageToScreen("Moving Platform State: Moving forward", Color::magenta, 5.0f, "moving_platform_debug_state");
	}

	Locator::getLog().LogMessageToScreen("Moving Platform Position: " + getOwner()->getPosition().toString(), Color::yellow, 5.0f, "moving_platform_debug_position");
	Locator::getLog().LogMessageToScreen("Moving Platform Timer: " + std::to_string(timer), Color::yellow, 5.0f, "moving_platform_debug_timer");
	Locator::getLog().LogMessageToScreen("Moving Platform Velocity: " + ECS::GetComponent(rigidbody).velocity.toString(), Color::yellow, 5.0f, "moving_platform_debug_velocity");
}