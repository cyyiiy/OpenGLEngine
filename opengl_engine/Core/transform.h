#pragma once

#include <Maths/vector3.h>
#include <Maths/quaternion.h>
#include <Maths/matrix4.h>

#include <Events/event.h>


class Transform
{
public:
	Transform();
	Transform(const Transform& other);


	void setPosition(Vector3 newPos);
	void setPosition(float newPosX, float newPosY, float newPosZ);
	void addPosition(Vector3 posAdd);
	//  add position to this transform that follows its rotation. Ideal for offsets
	void addPositionRotated(Vector3 posRotate);

	void setScale(Vector3 newScale, bool scaleInWorld = false);
	void setScale(float newScaleX, float newScaleY, float newScaleZ, bool scaleInWorld = false);
	void setScale(float newUniformScale, bool scaleInWorld = false);

	void setRotation(Quaternion newRotation);
	void incrementRotation(Quaternion increment);
	void rotateTowards(Vector3 posTowards);

	void pasteTransform(const Transform& transform);
	void clearTranform();


	const Vector3 getPosition() const { return position; }
	const Vector3 getScale() const { return scale; }
	const Quaternion getRotation() const { return rotation; }

	const Matrix4 getModelMatrix() const; 

	const Vector3 getForward() const;
	const Vector3 getUp() const;
	const Vector3 getRight() const;


	Event<> onTransformUpdated;

private:
	void computeMatrix();

	Vector3 position{ Vector3::zero };
	Vector3 scale{ Vector3::one };
	Quaternion rotation{ Quaternion::identity };

	Matrix4 modelMatrix{ Matrix4::identity };
};
