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
	lamp_model_comp.setModel(AssetManager::GetAsset<Model>(model_name));

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
	AssetManager::LoadAsset<Texture>("lamp_diffuse", { "doomlike/lamp/lamp_basecolor.png", false });
	AssetManager::LoadAsset<Texture>("lamp_specular", { "doomlike/lamp/lamp_roughness.png", false });

	AssetManager::LoadAsset<Texture>("chandelier_candle_diffuse", { "doomlike/chandelier/ch_candles_basecolor.jpeg", false });
	AssetManager::LoadAsset<Texture>("chandelier_base_diffuse", { "doomlike/chandelier/ch_chandelier_basecolor.jpeg", false });
	AssetManager::LoadAsset<Texture>("chandelier_base_specular", { "doomlike/chandelier/ch_chandelier_roughness.jpeg", false });
	AssetManager::LoadAsset<Texture>("chandelier_leather_diffuse", { "doomlike/chandelier/ch_leather_basecolor.jpeg", false });
	AssetManager::LoadAsset<Texture>("chandelier_leather_specular", { "doomlike/chandelier/ch_leather_roughness.jpeg", false });


	Material::LoadParams lamp_mat(AssetManager::GetAsset<Shader>("lit_object"));
	lamp_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("lamp_diffuse"));
	lamp_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("lamp_specular"));
	lamp_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	lamp_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("lamp", lamp_mat);

	Material::LoadParams chandelier_candle(AssetManager::GetAsset<Shader>("lit_object"));
	chandelier_candle.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("chandelier_candle_diffuse"));
	chandelier_candle.textures.emplace("specular", AssetManager::GetAsset<Texture>("default_black"));
	chandelier_candle.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	chandelier_candle.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("chandelier_candle", chandelier_candle);

	Material::LoadParams chandelier_base(AssetManager::GetAsset<Shader>("lit_object"));
	chandelier_base.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("chandelier_base_diffuse"));
	chandelier_base.textures.emplace("specular", AssetManager::GetAsset<Texture>("chandelier_base_specular"));
	chandelier_base.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	chandelier_base.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("chandelier_base", chandelier_base);

	Material::LoadParams chandelier_leather(AssetManager::GetAsset<Shader>("lit_object"));
	chandelier_leather.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("chandelier_leather_diffuse"));
	chandelier_leather.textures.emplace("specular", AssetManager::GetAsset<Texture>("lamp_specular"));
	chandelier_leather.textures.emplace("emissive", AssetManager::GetAsset<Texture>("chandelier_leather_specular"));
	chandelier_leather.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("chandelier_leather", chandelier_leather);

	Material::LoadParams flame(AssetManager::GetAsset<Shader>("flat_emissive"));
	flame.vec3Parameters.emplace("emissive", Color{ 209, 155, 67, 255 });
	AssetManager::LoadAsset<Material>("flame", flame);

	Material::LoadParams flame_off(AssetManager::GetAsset<Shader>("flat_emissive"));
	flame_off.vec3Parameters.emplace("emissive", Color{ 20, 14 ,3, 255 });
	AssetManager::LoadAsset<Material>("flame_off", flame_off);


	Model::FileImportParams lamp_model;
	lamp_model.modelPath = "doomlike/lamp/lamp.fbx";
	lamp_model.materials.push_back(AssetManager::GetAsset<Material>("lamp"));
	lamp_model.materials.push_back(AssetManager::GetAsset<Material>("flame"));
	AssetManager::LoadAsset<Model>("lamp", lamp_model);

	Model::FileImportParams chandelier_model;
	chandelier_model.modelPath = "doomlike/chandelier/chandelier.fbx";
	chandelier_model.materials.push_back(AssetManager::GetAsset<Material>("chandelier_base"));
	chandelier_model.materials.push_back(AssetManager::GetAsset<Material>("chandelier_leather"));
	chandelier_model.materials.push_back(AssetManager::GetAsset<Material>("flame")); // Could be "chandelier_candle" but "flame" allows a better visibility
	chandelier_model.materials.push_back(AssetManager::GetAsset<Material>("flame"));
	AssetManager::LoadAsset<Model>("lamp", chandelier_model);
}