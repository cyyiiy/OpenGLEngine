#include "lampFactory.h"
#include <Assets/assetManager.h>
#include <ECS/entityContainer.h>
#include <GameComponents/lampComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>


Entity* LampFactory::CreateLamp(EntityContainer* entityContainer, const Vector3& position, float intensityMultiplier, bool isCeiling, bool startOff)
{
	Entity* lamp_entity = entityContainer->createEntity();

	std::string model_name;
	Vector3 light_comp_offset;
	float light_comp_diffuse;
	Box col_comp_box;

	if (isCeiling)
	{
		// We must deal with models that are absolutely awful
		lamp_entity->setPosition(position + Vector3{ 0.0f, -2.0f, 0.0f });
		lamp_entity->setScale(0.001f);

		model_name = "chandelier";

		light_comp_offset = Vector3{ 0.0f, 550.0f, 0.0f };
		light_comp_diffuse = 0.41f;

		col_comp_box = Box{ Vector3{0.0f, 0.6f, 0.0f}, Vector3{0.5f, 0.5f, 0.5f} };
	}
	else
	{
		lamp_entity->setPosition(position + Vector3{ -2.58f, -1.23f, -1.52f });
		lamp_entity->setScale(0.012f);

		model_name = "lamp";

		light_comp_offset = Vector3{ 214.0f, 200.0f, 127.0f };
		light_comp_diffuse = 0.23f;

		col_comp_box = Box{ Vector3{2.58f, 1.87f, 1.52f}, Vector3{0.21f, 0.64f, 0.21f} };
	}


	ComponentHandle<ModelRendererComponent> lamp_model_handle = lamp_entity->addComponentByClass<ModelRendererComponent>();
	ModelRendererComponent& lamp_model_comp = ECS::GetComponent(lamp_model_handle);
	lamp_model_comp.setModel(&AssetManager::GetModel(model_name));

	ComponentHandle<PointLightComponent> lamp_light_handle = lamp_entity->addComponentByClass<PointLightComponent>();
	PointLightComponent& lamp_light_comp = ECS::GetComponent(lamp_light_handle);
	lamp_light_comp.lightColor = Color{ 227, 141, 2, 225 };
	lamp_light_comp.offset = light_comp_offset;
	lamp_light_comp.ambientStrength = 0.01f;
	lamp_light_comp.diffuseStrength = light_comp_diffuse * intensityMultiplier;
	lamp_light_comp.useColorToSpecular = true;

	BoxCollisionComponent& lamp_col_comp = ECS::GetComponent(lamp_entity->addComponentByClass<BoxCollisionComponent>());
	lamp_col_comp.collisionBox = col_comp_box;
	lamp_col_comp.collisionChannel = "solid";
	lamp_col_comp.useEntityScaleForBoxCenter = false;
	lamp_col_comp.useEntityScaleForBoxSize = false;

	LampComponent& lamp_comp = ECS::GetComponent(lamp_entity->addComponentByClass<LampComponent>());
	lamp_comp.setup(lamp_light_handle, lamp_model_handle, isCeiling);
	lamp_comp.changeStatus(!startOff);

	return lamp_entity;
}

void LampFactory::SetupLampAssets()
{
	AssetManager::LoadTexture("lamp_diffuse", "doomlike/lamp/lamp_basecolor.png", false);
	AssetManager::LoadTexture("lamp_specular", "doomlike/lamp/lamp_roughness.png", false);

	AssetManager::LoadTexture("chandelier_candle_diffuse", "doomlike/chandelier/ch_candles_basecolor.jpeg", false);
	AssetManager::LoadTexture("chandelier_base_diffuse", "doomlike/chandelier/ch_chandelier_basecolor.jpeg", false);
	AssetManager::LoadTexture("chandelier_base_specular", "doomlike/chandelier/ch_chandelier_roughness.jpeg", false);
	AssetManager::LoadTexture("chandelier_leather_diffuse", "doomlike/chandelier/ch_leather_basecolor.jpeg", false);
	AssetManager::LoadTexture("chandelier_leather_specular", "doomlike/chandelier/ch_leather_roughness.jpeg", false);

	Material& lamp_mat = AssetManager::CreateMaterial("lamp", AssetManager::GetShader("lit_object"));
	lamp_mat.addTexture(&AssetManager::GetTexture("lamp_diffuse"), TextureType::Diffuse);
	lamp_mat.addTexture(&AssetManager::GetTexture("lamp_specular"), TextureType::Specular);
	lamp_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	lamp_mat.addParameter("material.shininess", 32.0f);

	Material& chandelier_candle = AssetManager::CreateMaterial("chandelier_candle", AssetManager::GetShader("lit_object"));
	chandelier_candle.addTexture(&AssetManager::GetTexture("chandelier_candle_diffuse"), TextureType::Diffuse);
	chandelier_candle.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Specular);
	chandelier_candle.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	chandelier_candle.addParameter("material.shininess", 32.0f);
	Material& chandelier_base = AssetManager::CreateMaterial("chandelier_base", AssetManager::GetShader("lit_object"));
	chandelier_base.addTexture(&AssetManager::GetTexture("chandelier_base_diffuse"), TextureType::Diffuse);
	chandelier_base.addTexture(&AssetManager::GetTexture("chandelier_base_specular"), TextureType::Specular);
	chandelier_base.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	chandelier_base.addParameter("material.shininess", 32.0f);
	Material& chandelier_leather = AssetManager::CreateMaterial("chandelier_leather", AssetManager::GetShader("lit_object"));
	chandelier_leather.addTexture(&AssetManager::GetTexture("chandelier_leather_diffuse"), TextureType::Diffuse);
	chandelier_leather.addTexture(&AssetManager::GetTexture("chandelier_leather_specular"), TextureType::Specular);
	chandelier_leather.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	chandelier_leather.addParameter("material.shininess", 32.0f);

	Material& flame = AssetManager::CreateMaterial("flame", AssetManager::GetShader("flat_emissive"));
	flame.addParameter("emissive", Color{ 209, 155, 67, 255 });

	Material& flame_off = AssetManager::CreateMaterial("flame_off", AssetManager::GetShader("flat_emissive"));
	flame_off.addParameter("emissive", Color{ 20, 14 ,3, 255 });

	Model& lamp = AssetManager::LoadModel("lamp", "doomlike/lamp/lamp.fbx", nullptr);
	lamp.changeDefaultMaterial(0, &AssetManager::GetMaterial("lamp"));
	lamp.changeDefaultMaterial(1, &AssetManager::GetMaterial("flame"));

	Model& chandelier = AssetManager::LoadModel("chandelier", "doomlike/chandelier/chandelier.fbx", nullptr);
	chandelier.changeDefaultMaterial(0, &AssetManager::GetMaterial("chandelier_base"));
	chandelier.changeDefaultMaterial(1, &AssetManager::GetMaterial("chandelier_leather"));
	chandelier.changeDefaultMaterial(2, &AssetManager::GetMaterial("flame")); // Could be "chandelier_candle" but "flame" allows a better visibility
	chandelier.changeDefaultMaterial(3, &AssetManager::GetMaterial("flame"));
}

void LampFactory::ReleaseLampAssets()
{
	AssetManager::DeleteMaterial("lamp");
	AssetManager::DeleteMaterial("chandelier_candle");
	AssetManager::DeleteMaterial("chandelier_base");
	AssetManager::DeleteMaterial("chandelier_leather");
	AssetManager::DeleteMaterial("flame");
	AssetManager::DeleteMaterial("flame_off");
}