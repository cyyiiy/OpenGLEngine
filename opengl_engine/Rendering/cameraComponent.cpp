#include "cameraComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>
#include <Maths/Maths.h>


Matrix4 CameraComponent::getViewMatrix()
{
	return Matrix4::createLookAt(computedPos, computedPos + computedForward, computedUp);
}

Vector3 CameraComponent::getCamPosition() const
{
	return computedPos;
}

Vector3 CameraComponent::getCamForward() const
{
	return computedForward;
}

Vector3 CameraComponent::getCamUp() const
{
	return computedUp;
}

Vector3 CameraComponent::getCamRight() const
{
	return computedRight;
}

bool CameraComponent::isActiveCamera() const
{
	return Locator::getRenderer().IsActiveCamera(getSelfHandle<CameraComponent>());
}

void CameraComponent::setAsActiveCamera()
{
	Locator::getRenderer().SetCamera(getSelfHandle<CameraComponent>());
}

void CameraComponent::copyCamera(const CameraComponent& otherCamera, bool copyEntityTransform)
{
	posOffset = otherCamera.posOffset;
	rotOffset = otherCamera.rotOffset;
	yawOffset = otherCamera.yawOffset;
	pitchOffset = otherCamera.pitchOffset;
	rollOffset = otherCamera.rollOffset;
	fov = otherCamera.fov;

	if (copyEntityTransform)
	{
		getOwner()->pasteTransform(*(otherCamera.getOwner()));
	}

	computeCameraVectors(true, true);
}



void CameraComponent::addOffset(Vector3 offsetAdd)
{
	posOffset += offsetAdd;
	computeCameraVectors(true, false);
}

void CameraComponent::addYaw(float yawAdd)
{
	yawOffset += yawAdd;
	computeRotOffset();
}

void CameraComponent::addPitch(float pitchAdd)
{
	pitchOffset += pitchAdd;
	computeRotOffset();
}

void CameraComponent::addRoll(float rollAdd)
{
	rollOffset += rollAdd;
	computeRotOffset();
}

void CameraComponent::addFov(float fovAdd)
{
	fov += fovAdd;
}

void CameraComponent::setOffset(Vector3 offset_)
{
	posOffset = offset_;
	computeCameraVectors(true, false);
}

void CameraComponent::setYaw(float yaw_)
{
	yawOffset = yaw_;
	computeRotOffset();
}

void CameraComponent::setPitch(float pitch_)
{
	pitchOffset = pitch_;
	computeRotOffset();
}

void CameraComponent::setRoll(float roll_)
{
	rollOffset = roll_;
	computeRotOffset();
}

void CameraComponent::setFov(float fov_)
{
	fov = fov_;
}


void CameraComponent::init()
{
	Entity* owner = getOwner();
	if (!owner)
	{
		Locator::getLog().LogMessage_Category("Camera Component: A camera component was created without an owner!", LogCategory::Error);
		ECS::DeleteComponent(getSelfHandle<CameraComponent>());
		return;
	}

	owner->onTransformUpdated.subscribe(this, &CameraComponent::onEntityMoved);
	computeCameraVectors(true, true);
	setUpdateActivated(false);
}

void CameraComponent::exit()
{
	if (isActiveCamera())
	{
		Locator::getRenderer().RemoveActiveCamera();
	}
}

void CameraComponent::onEntityMoved()
{
	computeCameraVectors(true, true);
}

void CameraComponent::computeRotOffset()
{
	rotOffset = Quaternion::fromEuler(Maths::toRadians(yawOffset), Maths::toRadians(rollOffset), Maths::toRadians(pitchOffset));
	computeCameraVectors(false, true);
}

void CameraComponent::computeCameraVectors(bool computePos, bool computeDirections)
{
	if (computePos)
	{
		Matrix4 cam_pos_matrix = Matrix4::createTranslation(posOffset) * getOwner()->getModelMatrix();
		computedPos = cam_pos_matrix.getTranslation();
	}

	if (computeDirections)
	{
		Quaternion cam_rot = Quaternion::concatenate(rotOffset, getOwner()->getRotation());
		computedForward = Vector3::transform(Vector3::unitX, cam_rot);
		computedUp = Vector3::transform(Vector3::unitY, cam_rot);
		computedRight = Vector3::transform(Vector3::unitZ, cam_rot);
	}
}
