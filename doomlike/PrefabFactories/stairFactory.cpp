#include "stairFactory.h"
#include <Assets/assetManager.h>
#include <ECS/entityContainer.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <Rendering/Model/model.h>
#include <Rendering/texture.h>
#include <Rendering/material.h>
#include <vector>


Entity* StairFactory::CreateStair(EntityContainer* entityContainer, StairFacingDirection facingDirection, const Vector3& position)
{
	Entity* stair_entity = entityContainer->createEntity();

	stair_entity->setPosition(position + Vector3{ 0.0f, 1.f, 0.0f });

	std::vector<Box> collisions_boxes;
	collisions_boxes.resize(7);

	switch (facingDirection)
	{
	case StairFacingDirection::StairFacingPositiveX:
		// No need to rotate the stair entity

		for (int i = 0; i < 7; i++)
		{
			float num = float(i + 1);
			collisions_boxes[i] = Box{ Vector3{-0.125f * num, -1.125f + 0.250f * num, 0.0f}, Vector3{1.0f - 0.125f * num, 0.125f, 1.0f} };
		}
		break;

	case StairFacingDirection::StairFacingNegativeX:
		stair_entity->setRotation(Quaternion::fromEuler(Maths::toRadians(180.0f), 0.0f, 0.0f));

		for (int i = 0; i < 7; i++)
		{
			float num = float(i + 1);
			collisions_boxes[i] = Box{ Vector3{0.125f * num, -1.125f + 0.250f * num, 0.0f}, Vector3{1.0f - 0.125f * num, 0.125f, 1.0f} };
		}
		break;

	case StairFacingDirection::StairFacingPositiveZ:
		stair_entity->setRotation(Quaternion::fromEuler(Maths::toRadians(270.0f), 0.0f, 0.0f));

		for (int i = 0; i < 7; i++)
		{
			float num = float(i + 1);
			collisions_boxes[i] = Box{ Vector3{0.0f, -1.125f + 0.250f * num, -0.125f * num}, Vector3{1.0f, 0.125f, 1.0f - 0.125f * num} };
		}
		break;

	case StairFacingDirection::StairFacingNegativeZ:
		stair_entity->setRotation(Quaternion::fromEuler(Maths::toRadians(90.0f), 0.0f, 0.0f));

		for (int i = 0; i < 7; i++)
		{
			float num = float(i + 1);
			collisions_boxes[i] = Box{ Vector3{0.0f, -1.125f + 0.250f * num, 0.125f * num}, Vector3{1.0f, 0.125f, 1.0f - 0.125f * num} };
		}
		break;
	}

	ModelRendererComponent& stair_model_comp = ECS::GetComponent(stair_entity->addComponentByClass<ModelRendererComponent>());
	stair_model_comp.setModel(AssetManager::GetAsset<Model>("stairs"));
	stair_model_comp.offset.setScale(0.0044f);
	stair_model_comp.offset.setPosition(Vector3{ -1.03f, -1.11f, 0.93f });
	

	for (int i = 0; i < 7; i++)
	{
		BoxCollisionComponent& stair_step_col_comp = ECS::GetComponent(stair_entity->addComponentByClass<BoxCollisionComponent>());
		stair_step_col_comp.collisionBox = collisions_boxes[i];
		stair_step_col_comp.collisionChannel = "solid";
	}

	return stair_entity;
}



void StairFactory::SetupStairAssets()
{
	AssetManager::LoadAsset<Texture>("stairs_diffuse", { "doomlike/stairs/stairs_basecolor.png", false });
	AssetManager::LoadAsset<Texture>("stairs_specular", { "doomlike/stairs/stairs_fakespecular.jpg", false });

	Material::LoadParams stairs_mat(AssetManager::GetAsset<Shader>("lit_object"));
	stairs_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("stairs_diffuse"));
	stairs_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("stairs_specular"));
	stairs_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	stairs_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("stairs", stairs_mat);

	AssetManager::LoadAsset<Model>("stairs", Model::FileImportParams{ "doomlike/stairs/stairs.fbx", { AssetManager::GetAsset<Material>("stairs"), AssetManager::GetAsset<Material>("stairs"), AssetManager::GetAsset<Material>("stairs"), AssetManager::GetAsset<Material>("stairs"), AssetManager::GetAsset<Material>("stairs") } });
}