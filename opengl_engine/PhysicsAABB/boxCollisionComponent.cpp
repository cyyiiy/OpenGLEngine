#include "boxCollisionComponent.h"
#include "rigidbodyComponent.h"
#include <ECS/entity.h>


Box BoxCollisionComponent::getTransformedBox() const
{
	Box transformed_box;
	Entity& owner_entity = *getOwner();

	const Vector3 center_factor = useEntityScaleForBoxCenter ? owner_entity.getScale() : Vector3::one;
	const Vector3 scale_factor = useEntityScaleForBoxSize ? owner_entity.getScale() : Vector3::one;

	transformed_box.setCenterPoint((collisionBox.getCenterPoint() * center_factor) + owner_entity.getPosition());
	const Vector3 half_extents = collisionBox.getHalfExtents() * scale_factor;
	transformed_box.setHalfExtents(half_extents);

	return transformed_box;
}

Vector3 BoxCollisionComponent::getCenterDownPos() const
{
	const Box transformed_box = getTransformedBox();
	Vector3 center_down = transformed_box.getCenterPoint();
	center_down += Vector3{ 0.0f, -transformed_box.getHalfExtents().y, 0.0f };

	return center_down;
}

ComponentHandle<RigidbodyComponent> BoxCollisionComponent::getOwningRigidbody() const
{
	return owningRigidbody;
}