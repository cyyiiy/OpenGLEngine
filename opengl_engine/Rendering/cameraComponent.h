#pragma once
#include <ECS/behaviorComponent.h>
#include <Events/observer.h>
#include <Maths/Vector3.h>
#include <Maths/Matrix4.h>


/** Camera Component
* Component for the camera.
*/
class CameraComponent : public BehaviorComponent, public Observer
{
public:
	/**
	* Compute the view matrix of the camera for rendering.
	* Called automatically by the rendering process.
	*/
	Matrix4 getViewMatrix();

	/** Get the computed position of the camera. */
	Vector3 getCamPosition() const;

	/** Get the computed forward vector of the camera. */
	Vector3 getCamForward() const;

	/** Get the computed up vector of the camera. */
	Vector3 getCamUp() const;

	/** Get the computed right vector of the camera. */
	Vector3 getCamRight() const;

	/** Check if this camera is the active one used by the renderer. */
	bool isActiveCamera() const;

	/** Set this camera as the active one used by the renderer. */
	void setAsActiveCamera();

	/**
	* Copy another camera to have the same offsets, fov, etc.
	* @param	otherCamera				The camera to copy.
	* @param	copyEntityTransform		If true, also copy the transform of the entity of the other camera to this camera's entity.
	*/
	void copyCamera(const CameraComponent& otherCamera, bool copyEntityTransform);


	void addOffset(Vector3 offsetAdd);
	void addYaw(float yawAdd);
	void addPitch(float pitchAdd);
	void addRoll(float rollAdd);
	void addFov(float fovAdd);

	void setOffset(Vector3 offset_);
	void setYaw(float yaw_);
	void setPitch(float pitch_);
	void setRoll(float roll_);
	void setFov(float fov_);

	Vector3 getOffset() const { return posOffset; }
	float getYaw() const { return yawOffset; }
	float getPitch() const { return pitchOffset; }
	float getRoll() const { return rollOffset; }
	Quaternion getRotOffset() const { return rotOffset; }
	float getFov() const { return fov; }

protected:
	virtual void init() override;
	virtual void exit() override;

	void onEntityMoved();
	void computeRotOffset();
	virtual void computeCameraVectors(bool computePos, bool computeDirections);

	Vector3 posOffset{ Vector3::zero };
	Quaternion rotOffset{ Quaternion::identity };

	float yawOffset{ 0.0f };
	float pitchOffset{ 0.0f };
	float rollOffset{ 0.0f };

	float fov{ 45.0f };


	Vector3 computedPos{ Vector3::zero };
	Vector3 computedForward{ Vector3::unitX };
	Vector3 computedUp{ Vector3::unitY };
	Vector3 computedRight{ Vector3::unitZ };
};


// Specify sublist size for 'CameraComponent'
template<>
struct ComponentSublistSize<CameraComponent>
{
	static constexpr size_t value = 4;
};