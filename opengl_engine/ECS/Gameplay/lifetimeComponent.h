#pragma once
#include <ECS/component.h>


/** Lifetime Component
* A component that will delete its entity after its timer reaches 0.
*/
class LifetimeComponent : public Component
{
public:
	/** The lifetime timer. When it reaches 0, delete the entity this component is on. */
	float lifetimeTimer{ 0.0f };
};