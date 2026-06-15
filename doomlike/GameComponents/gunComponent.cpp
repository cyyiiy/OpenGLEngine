#include "gunComponent.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <GameplayStatics/gameplayStatics.h>
#include <Inputs/Input.h>

#include <GameComponents/playerComponent.h>
#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Text/textComponent.h>
#include <Rendering/Hud/spriteComponent.h>
#include <GameComponents/bulletComponent.h>


void GunComponent::reset()
{
	if (!gunValid) return;

	for (auto& active_bullet : activeBullets)
	{
		ECS::GetComponent(active_bullet).deleteBullet();
	}
	activeBullets.clear();

	reloadTimer = 0.0f;
	ammoCount = MAX_AMMO;
	writeAmmoText();
}

void GunComponent::init()
{
	entity = getOwner();

	player = entity->getComponentOfClass<PlayerComponent>();
	if (!ECS::IsComponentHandleValid(player))
	{
		Locator::getLog().LogMessage_Category("FPS Demo: A gun component was added on an entity that doesn't have a Player Component!", LogCategory::Error);
		setUpdateActivated(false);
		return;
	}
	entity->onTransformUpdated.subscribe(this, &GunComponent::onPlayerTransformUpdated);
	gunValid = true;

	gunModel = entity->addComponentByClass<ModelRendererComponent>();
	ModelRendererComponent& gun_model_comp = ECS::GetComponent(gunModel);
	gun_model_comp.model = &AssetManager::GetModel("gun");
	gun_model_comp.ignoreOwnerTransform = true;
	gun_model_comp.offset.setScale(0.1f);

	ammoText = entity->addComponentByClass<TextComponent>();
	TextComponent& ammo_text_comp = ECS::GetComponent(ammoText);
	ammo_text_comp.setTextDatas("", AssetManager::GetFont("arial_64"));
	ammo_text_comp.position = HudPosition{ Vector2::zero, Vector2::zero, Vector2{ 50.0f, 50.0f } }; // Bottom left with an offset
	ammo_text_comp.scale = Vector2{ 0.6f };
	writeAmmoText();

	crosshairSprite = entity->addComponentByClass<SpriteComponent>();
	SpriteComponent& crosshair_sprite_comp = ECS::GetComponent(crosshairSprite);
	crosshair_sprite_comp.texture = &AssetManager::GetTexture("hud_crosshair");
	crosshair_sprite_comp.position = HudPosition{ Vector2::halfUnit, Vector2::halfUnit, Vector2::zero }; // Center with no offset
	crosshair_sprite_comp.scale = Vector2{ 0.5f };
}

void GunComponent::exit()
{
	for (auto& active_bullet : activeBullets)
	{
		if (ECS::IsComponentHandleValid(active_bullet)) ECS::GetComponent(active_bullet).deleteBullet();
	}
	activeBullets.clear();
}

void GunComponent::update(float deltaTime)
{
	if (!gunValid) return;

	// Shoot
	if (Input::IsKeyPressed(GLFW_MOUSE_BUTTON_LEFT) && ammoCount > 0 && reloadTimer == 0.0f)
	{
		ModelRendererComponent& gun_model_comp = ECS::GetComponent(gunModel);

		// Shoot a raycast towards the camera direction to shoot the bullet towards the center of the screen even if it starts from the bottom right
		Physics& physics = Locator::getPhysics();
		RaycastHitInfos raycast_out;
		CameraComponent& player_camera = ECS::GetComponent(ECS::GetComponent(player).camera);
		const Vector3 raycast_target = player_camera.getCamPosition() + player_camera.getCamForward() * 1000.0f;
		bool raycast_hit = physics.LineRaycast(player_camera.getCamPosition(), raycast_target, { "solid", "enemy" }, raycast_out, 1.0f);

		Quaternion bullet_rotation;
		Vector3 bullet_direction;
		if (!raycast_hit)
		{
			bullet_rotation = Quaternion::createLookAt(gun_model_comp.offset.getPosition(), raycast_target, Vector3::unitY);
			bullet_direction = player_camera.getCamForward();
		}
		else
		{
			bullet_rotation = Quaternion::createLookAt(gun_model_comp.offset.getPosition(), raycast_out.hitLocation, Vector3::unitY);
			bullet_direction = Vector3::normalize(raycast_out.hitLocation - gun_model_comp.offset.getPosition());
		}

		Entity* bullet_entity = GameplayStatics::GetGame()->createEntity();
		activeBullets.push_back(bullet_entity->addComponentByClass<BulletComponent>());
		ECS::GetComponent(activeBullets.back()).setupBullet(gun_model_comp.offset.getPosition(), bullet_rotation, bullet_direction, SHOOT_VELOCITY, BULLET_LIFETIME);

		// Play shoot sound
		Locator::getAudio().InstantPlaySound2D(AssetManager::GetSound("shoot"), 0.15f);

		ammoCount--;
		writeAmmoText();
	}


	// Reload
	if (Input::IsKeyPressed(GLFW_KEY_R) && ammoCount < MAX_AMMO && reloadTimer == 0.0f)
	{
		reloadTimer = RELOAD_DURATION;
	}

	if (reloadTimer > 0.0f)
	{
		reloadTimer -= deltaTime;
		if (reloadTimer <= 0.0f)
		{
			reloadTimer = 0.0f;
			ammoCount = MAX_AMMO;
			writeAmmoText();
		}
	}


	// Delete expired bullets
	for (int i = 0; i < activeBullets.size(); i++)
	{
		BulletComponent& bullet = ECS::GetComponent(activeBullets[i]);

		if (!bullet.isLifetimeOver()) continue;

		bullet.deleteBullet();

		std::iter_swap(activeBullets.begin() + i, activeBullets.end() - 1);
		activeBullets.pop_back();

		break; // Assume that we can't create 2 bullets on the same frame so 2 bullets cannot be destroyed by time out on the same frame
	}
}

void GunComponent::onPlayerTransformUpdated()
{
	if (!gunValid) return;

	ModelRendererComponent& gun_model_comp = ECS::GetComponent(gunModel);
	CameraComponent& player_camera = ECS::GetComponent(ECS::GetComponent(player).camera);

	Transform& gun_offset = gun_model_comp.offset;
	gun_offset.setPosition(player_camera.getCamPosition());
	gun_offset.setRotation(Quaternion::concatenate(player_camera.getRotOffset(), entity->getRotation()));
	gun_offset.incrementRotation(Quaternion{ gun_offset.getUp(), Maths::toRadians(180.0f) }); //  gun is rotated backward by default
	gun_offset.addPositionRotated(Vector3{ 0.1f, -0.1f, -0.2f }); //  gun offset of camera
	gun_offset.incrementRotation(Quaternion{ gun_offset.getRight(), Maths::toRadians(reloadTimer * 720.0f) });
}

void GunComponent::writeAmmoText()
{
	if (!gunValid) return;

	TextComponent& ammo_text_comp = ECS::GetComponent(ammoText);
	ammo_text_comp.setText("Ammo: " + std::to_string(ammoCount) + "/" + std::to_string(MAX_AMMO));
}
