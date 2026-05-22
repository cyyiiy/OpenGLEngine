#pragma once
#include <Physics/collisionComponent.h>
#include <Maths/Geometry/box.h>
#include <Events/observer.h>

struct CollisionResponse;


/** Box AABB Collision Component
* Component for a AABB collision component
* This type of collision support position and scale from transform, but not rotation.
*/
class BoxAABBColComp : public CollisionComponent, public Observer
{
// ----------------------------------------------------------
//  Axis Aligned Bounding Box
// ----------------------------------------------------------
public:
	/**
	* Set the box values of this AABB collision.
	* @param	boxValues	The new box values to set to this AABB collision component.
	*/
	void setBox(const Box& boxValues);

	/**
	* Get the box transformed by the owner's transform.
	* Reminder: AABB can be transformed on position and scale, but not rotation.
	* @return					The AABB transformed by the owner.
	*/
	Box getTransformedBox() const;

	/**
	* Get the normal of the surface the given point is on.
	* If the given point isn't on the surface of the transformed box, return Vector3::zero
	* @param	point	The point on the surface of the transformed box you want to get the normal.
	* @return			The normal of the surface the given point is on.
	*/
	Vector3 getNormal(const Vector3& point) const;


// ----------------------------------------------------------
//  AABB Collision getters
// ----------------------------------------------------------
public:
	Box getRawBox() const;

	const Matrix4 getModelMatrix() const override;
	Vector3 getCenterDownPos() const override;
	Box getEncapsulatingBox() const override;


// ----------------------------------------------------------
//  Audio Collision (wip, not working for the moment)
// ----------------------------------------------------------
public:
	/**
	* Setup this collision as an audio collision.
	* Warning: Audio Collision is not working in the engine for the moment. It will most likely have no effect.
	* @param	AudioCollisionOcclusion		The datas for the audio collision.
	*/
	void setupAudioCollision(const AudioCollisionOcclusion& audioCollisionType) override;

private:
	void onOwnerTransformUpdated();


// ----------------------------------------------------------
//  Public properties (owner transform settings)
// ----------------------------------------------------------
public:
	bool useOwnerScaleForBoxSize{ true };
	bool useOwnerScaleForBoxCenter{ true };



// ----------------------------------------------------------
//  Overriden functions from parent collision component
// ----------------------------------------------------------
protected:
	bool resolvePointIntersection(const Vector3& point) const override;
	bool resolveLineRaycastIntersection(const Ray& raycast, RaycastHitInfos& outHitInfos) const override;
	bool resolveAABBRaycastIntersection(const Box& raycast) const override;
	bool resolveAABBSweepRaycastIntersection(const Ray& raycast, const Box& boxRaycast, RaycastHitInfos& outHitInfos, bool forCollisionTest) const override;

	void drawDebugMesh(Material& debugMaterial) const override;


// ----------------------------------------------------------
//  Initialize function (called at component creation)
// ----------------------------------------------------------
protected:
	void init() override;



// ----------------------------------------------------------
//  Private properties (Box)
// ----------------------------------------------------------
private:
	Box box{ Box::one };
};
