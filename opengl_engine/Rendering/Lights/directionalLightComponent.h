#pragma once
#include "lightBase.h"
#include <ECS/component.h>
#include <Maths/Vector3.h>


/** Directional Light Component
* A data-only component for a directional light.
*/
class DirectionalLightComponent : public Component, public LightBase
{
public:
	/** The direction of the directional light. */
	Vector3 direction{ Vector3::unitX };
};


// Specify sublist size for 'DirectionalLightComponent'
template<>
struct ComponentSublistSize<DirectionalLightComponent>
{
	static constexpr size_t value = 1;
};