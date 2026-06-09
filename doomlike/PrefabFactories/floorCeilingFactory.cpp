#include "floorCeilingFactory.h"
#include <Assets/assetManager.h>
#include <ECS/entityContainer.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>


Entity* FloorCeilingFactory::CreateFloor(EntityContainer* entityContainer, const Vector3& position, const Vector2& scale, bool isWood, bool createCollision)
{
	Entity* floor_entity = entityContainer->createEntity();

	floor_entity->setPosition(position);
	floor_entity->setScale(scale.x, 1.0f, scale.y);

	ModelRendererComponent& floor_model_comp = ECS::GetComponent(floor_entity->addComponentByClass<ModelRendererComponent>());
	floor_model_comp.model = &AssetManager::GetModel(isWood ? "floor_wood" : "floor");

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
	ceiling_model_comp.model = &AssetManager::GetModel("ceiling");

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
	AssetManager::LoadTexture("floor_stone_diffuse", "doomlike/textures/stone_floor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_stone_specular", "doomlike/textures/stone_floor_specular.jpg", false);

	AssetManager::LoadTexture("floor_wood_diffuse", "doomlike/textures/wood_floor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_wood_specular", "doomlike/textures/wood_floor_specular.jpg", false);

	AssetManager::LoadTexture("ceiling_diffuse", "doomlike/textures/wood_ceiling_basecolor.jpg", false);
	AssetManager::LoadTexture("ceiling_specular", "doomlike/textures/wood_ceiling_specular.jpg", false);

	Material& floor_mat = AssetManager::CreateMaterial("floor", AssetManager::GetShader("lit_object"));
	floor_mat.addTexture(&AssetManager::GetTexture("floor_stone_diffuse"), TextureType::Diffuse);
	floor_mat.addTexture(&AssetManager::GetTexture("floor_stone_specular"), TextureType::Specular);
	floor_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_mat.addParameter("material.shininess", 32.0f);
	floor_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_mat.addParameter("beta_tex_scaling_factor", 2.0f);

	Material& floor_wood_mat = AssetManager::CreateMaterial("floor_wood", AssetManager::GetShader("lit_object"));
	floor_wood_mat.addTexture(&AssetManager::GetTexture("floor_wood_diffuse"), TextureType::Diffuse);
	floor_wood_mat.addTexture(&AssetManager::GetTexture("floor_wood_specular"), TextureType::Specular);
	floor_wood_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_wood_mat.addParameter("material.shininess", 20.0f);
	floor_wood_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_wood_mat.addParameter("beta_tex_scaling_factor", 2.0f);

	Material& ceiling_mat = AssetManager::CreateMaterial("ceiling", AssetManager::GetShader("lit_object"));
	ceiling_mat.addTexture(&AssetManager::GetTexture("ceiling_diffuse"), TextureType::Diffuse);
	ceiling_mat.addTexture(&AssetManager::GetTexture("ceiling_specular"), TextureType::Specular);
	ceiling_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	ceiling_mat.addParameter("material.shininess", 32.0f);
	ceiling_mat.addParameter("beta_prevent_tex_scaling", true);
	ceiling_mat.addParameter("beta_tex_scaling_factor", 2.0f);

	AssetManager::CreateModel("floor");
	AssetManager::GetModel("floor").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor"));

	AssetManager::CreateModel("floor_wood");
	AssetManager::GetModel("floor_wood").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor_wood"));

	AssetManager::CreateModel("ceiling");
	AssetManager::GetModel("ceiling").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("ceiling"));
}

void FloorCeilingFactory::ReleaseFloorCeilingAssets()
{
	AssetManager::DeleteMaterial("floor");
	AssetManager::DeleteMaterial("floor_wood");
	AssetManager::DeleteMaterial("ceiling");
}
