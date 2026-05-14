#pragma once
#include <ECS/component.h>
#include <Rendering/Hud/hudElement.h>
#include <Rendering/texture.h>


/** Sprite Component
* A data-only component that holds informations to render a hud sprite.
* Note: HUD elements don't use their entity's transform.
*/
class SpriteComponent : public Component, public HudElement
{
public:
	/** The sprite texture to render. */
	Texture* texture{ nullptr };
};


// Specify sublist size for 'SpriteComponent'
template<>
struct ComponentSublistSize<SpriteComponent>
{
	static constexpr size_t value = 16;
};