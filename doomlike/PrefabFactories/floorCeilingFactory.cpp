#include "floorCeilingFactory.h"
#include <Assets/assetManager.h>
#include <ECS/entityContainer.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <Rendering/Model/model.h>
#include <Rendering/texture.h>
#include <Rendering/material.h>


Entity* FloorCeilingFactory::CreateFloor(EntityContainer* entityContainer, const Vector3& position, const Vector2& scale, bool isWood, bool createCollision)
{
	Entity* floor_entity = entityContainer->createEntity();

	floor_entity->setPosition(position);
	floor_entity->setScale(scale.x, 1.0f, scale.y);

	ModelRendererComponent& floor_model_comp = ECS::GetComponent(floor_entity->addComponentByClass<ModelRendererComponent>());
	floor_model_comp.setModel(AssetManager::GetAsset<Model>("default_plane"));
	floor_model_comp.setMaterial(AssetManager::GetAsset<Material>(isWood ? "floor_wood" : "floor"), 0);

	if (createCollision)
	{
		BoxCollisionComponent& floor_col_comp = ECS::GetComponent(floor_entity->addComponentByClass<BoxCollisionComponent>());
		floor_col_comp.collisionBox = Box{ Vector3{0.0f, -0.1f, 0.0f}, Vector3{0.5f, 0.1f, 0.5f} };
		floor_col_comp.collisionChannel = "solid";
	}

	return floor_entity;
}

Entity* FloorCeilingFactory::CreateCeiling(EntityContainer* entityContainer, const Vector3& position, const Vector2& scale, bool createCollision)
{
	Entity* ceiling_entity = entityContainer->createEntity();

	ceiling_entity->setPosition(position);
	ceiling_entity->setScale(scale.x, 1.0f, scale.y);
	ceiling_entity->setRotation(Quaternion::fromEuler(0.0f, Maths::toRadians(180.0f), 0.0f));

	ModelRendererComponent& ceiling_model_comp = ECS::GetComponent(ceiling_entity->addComponentByClass<ModelRendererComponent>());
	ceiling_model_comp.setModel(AssetManager::GetAsset<Model>("default_plane"));
	ceiling_model_comp.setMaterial(AssetManager::GetAsset<Material>("ceiling"), 0);

	if (createCollision)
	{
		BoxCollisionComponent& ceiling_col_comp = ECS::GetComponent(ceiling_entity->addComponentByClass<BoxCollisionComponent>());
		ceiling_col_comp.collisionBox = Box{ Vector3{0.0f, 0.2f, 0.0f}, Vector3{0.5f, 0.2f, 0.5f} };
		ceiling_col_comp.collisionChannel = "solid";
	}

	return ceiling_entity;
}



void FloorCeilingFactory::SetupFloorCeilingAssets()
{
	AssetManager::LoadAsset<Texture>("floor_stone_diffuse", { "doomlike/textures/stone_floor_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("floor_stone_specular", { "doomlike/textures/stone_floor_specular.jpg", false });

	AssetManager::LoadAsset<Texture>("floor_wood_diffuse", { "doomlike/textures/wood_floor_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("floor_wood_specular", { "doomlike/textures/wood_floor_specular.jpg", false });

	AssetManager::LoadAsset<Texture>("ceiling_diffuse", { "doomlike/textures/wood_ceiling_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("ceiling_specular", { "doomlike/textures/wood_ceiling_specular.jpg", false });

	Material::LoadParams floor_mat(AssetManager::GetAsset<Shader>("lit_object"));
	floor_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("floor_stone_diffuse"));
	floor_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("floor_stone_specular"));
	floor_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	floor_mat.floatParameters.emplace("material.shininess", 32.0f);
	floor_mat.boolParameters.emplace("beta_prevent_tex_scaling", true);
	floor_mat.floatParameters.emplace("beta_tex_scaling_factor", 2.0f);
	AssetManager::LoadAsset<Material>("floor", floor_mat);

	Material::LoadParams floor_wood_mat(AssetManager::GetAsset<Shader>("lit_object"));
	floor_wood_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("floor_wood_diffuse"));
	floor_wood_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("floor_wood_specular"));
	floor_wood_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	floor_wood_mat.floatParameters.emplace("material.shininess", 20.0f);
	floor_wood_mat.boolParameters.emplace("beta_prevent_tex_scaling", true);
	floor_wood_mat.floatParameters.emplace("beta_tex_scaling_factor", 2.0f);
	AssetManager::LoadAsset<Material>("floor_wood", floor_wood_mat);

	Material::LoadParams ceiling_mat(AssetManager::GetAsset<Shader>("lit_object"));
	ceiling_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("ceiling_diffuse"));
	ceiling_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("ceiling_specular"));
	ceiling_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	ceiling_mat.floatParameters.emplace("material.shininess", 32.0f);
	ceiling_mat.boolParameters.emplace("beta_prevent_tex_scaling", true);
	ceiling_mat.floatParameters.emplace("beta_tex_scaling_factor", 2.0f);
	AssetManager::LoadAsset<Material>("ceiling", ceiling_mat);
}