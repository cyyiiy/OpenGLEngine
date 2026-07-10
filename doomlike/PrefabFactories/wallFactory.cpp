#include "wallFactory.h"
#include <Assets/assetManager.h>
#include <ECS/entityContainer.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>


Entity* WallFactory::CreateWall(EntityContainer* entityContainer, WallFacingDirection facingDirection, const Vector3& position, const Vector2& scale, bool isAltTex, bool createCollision)
{
	Entity* wall_entity = entityContainer->createEntity();

	wall_entity->setPosition(position);
	wall_entity->setScale(scale.x, 1.0f, scale.y);

	const Vector2 scale_col = scale * 0.5f;
	Box wall_col_box;
	switch (facingDirection)
	{
	case WallFacingDirection::WallFacingPositiveX:
		wall_entity->setRotation(Quaternion::fromEuler(0.0f, Maths::toRadians(90.0f), Maths::toRadians(-90.0f)));
		if (createCollision)
			wall_col_box = Box{ Vector3{-0.1f, 0.0f, 0.0f}, Vector3{0.1f, scale_col.y, scale_col.x} };
		break;

	case WallFacingDirection::WallFacingNegativeX:
		wall_entity->setRotation(Quaternion::fromEuler(0.0f, Maths::toRadians(90.0f), Maths::toRadians(90.0f)));
		if (createCollision)
			wall_col_box = Box{ Vector3{0.1f, 0.0f, 0.0f}, Vector3{0.1f, scale_col.y, scale_col.x} };
		break;

	case WallFacingDirection::WallFacingPositiveZ:
		wall_entity->setRotation(Quaternion::fromEuler(Maths::toRadians(90.0f), Maths::toRadians(90.0f), Maths::toRadians(90.0f)));
		if (createCollision)
			wall_col_box = Box{ Vector3{0.0f, 0.0f, -0.1f}, Vector3{scale_col.x, scale_col.y, 0.1f} };
		break;

	case WallFacingDirection::WallFacingNegativeZ:
		wall_entity->setRotation(Quaternion::fromEuler(Maths::toRadians(-90.0f), Maths::toRadians(90.0f), Maths::toRadians(90.0f)));
		if (createCollision)
			wall_col_box = Box{ Vector3{0.0f, 0.0f, 0.1f}, Vector3{scale_col.x, scale_col.y, 0.1f} };
		break;
	}

	ModelRendererComponent& wall_model_comp = ECS::GetComponent(wall_entity->addComponentByClass<ModelRendererComponent>());
	wall_model_comp.setModel(&AssetManager::GetModel("default_plane"));
	wall_model_comp.setMaterial(&AssetManager::GetMaterial(isAltTex ? "wall_alt" : "wall"), 0);

	if (createCollision)
	{
		BoxCollisionComponent& wall_col_comp = ECS::GetComponent(wall_entity->addComponentByClass<BoxCollisionComponent>());
		wall_col_comp.collisionBox = wall_col_box;
		wall_col_comp.collisionChannel = "solid";
		wall_col_comp.useEntityScaleForBoxCenter = false;
		wall_col_comp.useEntityScaleForBoxSize = false;
	}

	return wall_entity;
}



void WallFactory::SetupWallAssets()
{
	AssetManager::LoadTexture("wall_diffuse", "doomlike/textures/stone_wall_basecolor.jpg", false);
	AssetManager::LoadTexture("wall_specular", "doomlike/textures/stone_wall_specular.jpg", false);

	AssetManager::LoadTexture("wall_alt_diffuse", "doomlike/textures/concrete_wall_basecolor.jpg", false);
	AssetManager::LoadTexture("wall_alt_specular", "doomlike/textures/concrete_wall_specular.jpg", false);

	Material& wall_mat = AssetManager::CreateMaterial("wall", AssetManager::GetShader("lit_object"));
	wall_mat.addTexture(&AssetManager::GetTexture("wall_diffuse"), TextureType::Diffuse);
	wall_mat.addTexture(&AssetManager::GetTexture("wall_specular"), TextureType::Specular);
	wall_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	wall_mat.addParameter("material.shininess", 10.0f);
	wall_mat.addParameter("beta_prevent_tex_scaling", true);
	wall_mat.addParameter("beta_tex_scaling_factor", 2.0f);

	Material& wall_alt_mat = AssetManager::CreateMaterial("wall_alt", AssetManager::GetShader("lit_object"));
	wall_alt_mat.addTexture(&AssetManager::GetTexture("wall_alt_diffuse"), TextureType::Diffuse);
	wall_alt_mat.addTexture(&AssetManager::GetTexture("wall_alt_specular"), TextureType::Specular);
	wall_alt_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	wall_alt_mat.addParameter("material.shininess", 10.0f);
	wall_alt_mat.addParameter("beta_prevent_tex_scaling", true);
	wall_alt_mat.addParameter("beta_tex_scaling_factor", 2.0f);
}

void WallFactory::ReleaseWallAssets()
{
	AssetManager::DeleteMaterial("wall");
	AssetManager::DeleteMaterial("wall_alt");
}