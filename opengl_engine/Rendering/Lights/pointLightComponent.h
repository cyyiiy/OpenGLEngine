#pragma once
#include "positionBasedLight.h"


/** Point Light Component
* A data-only component for a point light.
*/
class PointLightComponent : public PositionBasedLight
{
public:
	/** Wether or not to use the diffuse color for the specular color (default specular color is white). */
	bool useColorToSpecular{ false };
};


// Specify sublist size for 'PointLightComponent'
template<>
struct ComponentSublistSize<PointLightComponent>
{
	static constexpr size_t value = 32;
};