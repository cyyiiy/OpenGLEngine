#pragma once
#include <ECS/BehaviorComponent.h>
#include <Maths/Vector3.h>

class BoxCollisionComponent;
class RigidbodyComponent;
class AudioSourceComponent;


/**
* Add this component to an entity to make it a moving platform.
* It automatically create the collision, the rigidbody and the audio component, but not the model renderer.
*/
class MovingPlatformComponent : public BehaviorComponent
{
public:
	void setupMovingPlatform(const Vector3& pointA_, const Vector3& pointB_, const float movementSpeed_, const float waitTime_ = 0.0f, const bool debug_ = false);

	void pauseMovement();
	void resumeMovement();

	void init() override;
	void update(float dt) override;

private:
	ComponentHandle<BoxCollisionComponent> collision;
	ComponentHandle<RigidbodyComponent> rigidbody;
	ComponentHandle<AudioSourceComponent> audioSource;

	Vector3 pointA{ Vector3::zero };
	Vector3 pointB{ Vector3::zero };
	float timeAtoB{ 0.0f };
	float waitTime{ 0.0f };

	float timer{ 0.0f };
	float waitTimer{ 0.0f };
	bool reverse{ false };
	bool waiting{ false };
	bool paused{ false };

	bool debug{ false };

	virtual void updateDebug(float dt);
};

