#pragma once
#include "positionBasedLight.h"


/** Spot Light Cutoff
* Defines the cutoffs of the spot light.
*/
struct SpotLightCutoffs
{
	float innerCutoff;
	float outerCutoff;
};


/** Spot Light Component
* A data-only component for a spot light.
*/
class SpotLightComponent : public PositionBasedLight
{
public:
	/** The direction of the spot light. */
	Vector3 direction{ Vector3::unitX };

	/** The inner and outer cutoffs of the spot light. Defaults to a ~45° angle. */
	SpotLightCutoffs cutoffs{ 0.766f, 0.707f };

	/** Wether or not to use the diffuse color for the specular color (default specular color is white). */
	bool useColorToSpecular{ false };
};


// Specify sublist size for 'SpotLightComponent'
template<>
struct ComponentSublistSize<SpotLightComponent>
{
	static constexpr size_t value = 16;
};