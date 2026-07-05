#pragma once
#include <ECS/BehaviorComponent.h>
#include <Events/observer.h>
#include <vector>

class PlayerComponent;
class ModelRendererComponent;
class TextComponent;
class SpriteComponent;


/**
* Component to add to an entity with the player component to add it a gun.
* It automatically create the needed components on its entity.
*/
class GunComponent : public BehaviorComponent, public Observer
{
	const int MAX_AMMO = 5;
	const float RELOAD_DURATION = 0.5f;
	const float SHOOT_VELOCITY = 15.0f;
	const float BULLET_LIFETIME = 3.0f;

public:
	void reset();

	void init() override;
	void update(float dt) override;

	void onPlayerTransformUpdated();

private:
	Entity* entity{ nullptr };
	ComponentHandle<PlayerComponent> player;
	ComponentHandle<ModelRendererComponent> gunModel;
	ComponentHandle<TextComponent> ammoText;
	ComponentHandle<SpriteComponent> crosshairSprite;

	bool gunValid{ false };
	int ammoCount{ 0 };
	float reloadTimer{ 0.0f };

	void writeAmmoText();
};


// Specify sublist size for 'GunComponent'
template<>
struct ComponentSublistSize<GunComponent>
{
	static constexpr size_t value = 1;
};