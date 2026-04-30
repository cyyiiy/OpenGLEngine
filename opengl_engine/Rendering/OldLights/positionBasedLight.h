#pragma once
#include "lightComponent.h"
#include <Events/observer.h>
#include <Maths/Vector3.h>

/** Position Based Light
* Base class for light types that exists in the scene (point and spot lights).
* Regroup functionnalities of offset to Entity and falloff.
* Abstract Component - do not add it to an Entity.
*/
class PositionBasedLight : public LightComponent, public Observer
{
public:
	/** Initialize the event bind to the Entity transform. */
	void initializePosition();

	/** Get the computed position of the light. */
	Vector3 getPosition() const;


	void setFalloff(float newConstant, float newLinear, float newQuadratic);

	void setOffset(Vector3 newOffset);
	void setConstant(float newConstant);
	void setLinear(float newLinear);
	void setQuadratic(float newQuadratic);

	Vector3 getOffset() const;
	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;

protected:
	void exit() override;
	void onEntityMoved();
	void recomputePosition();
	void resetValues();

	Vector3 offset{ Vector3::zero };
	Vector3 position{ Vector3::zero };

	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
};

