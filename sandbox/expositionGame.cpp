#include "expositionGame.h"
#include <ServiceLocator/locator.h>
#include <Assets/defaultAssets.h>
#include <Assets/assetManager.h>
#include <Rendering/texture.h>
#include <Rendering/material.h>
#include <Rendering/Model/model.h>
#include <Rendering/Text/font.h>
#include <Audio/audioSound.h>


ExpositionGame::ExpositionGame()
{
}

void ExpositionGame::updateGame(float dt)
{
}


void ExpositionGame::loadGameAssets()
{
	Locator::getLog().SetConsoleLogDisplayRule(LogCategory::Info);

	DefaultAssets::LoadDefaultAssets();

	// Load textures and materials
	AssetManager::LoadAsset<Texture>("container_diffuse", { "container2.png", false });
	AssetManager::LoadAsset<Texture>("container_specular", { "container2_specular.png", false });
	AssetManager::LoadAsset<Texture>("matrix_emissive", { "matrix.jpg", false });

	AssetManager::LoadAsset<Texture>("backpack_diffuse", { "backpack/backpack_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("backpack_specular", { "backpack/backpack_specular.jpg", false });

	AssetManager::LoadAsset<Texture>("floor_wood_diffuse", { "doomlike/textures/wood_floor_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("floor_wood_specular", { "doomlike/textures/wood_floor_specular.jpg", false });

	AssetManager::LoadAsset<Texture>("smileyface_sprite", { "awesomeface.png", false });

	Material::LoadParams container_mat(AssetManager::GetAsset<Shader>("lit_object"));
	container_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("container_diffuse"));
	container_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("container_specular"));
	container_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	container_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("container", container_mat);

	Material::LoadParams container_matrix_mat(AssetManager::GetAsset<Shader>("lit_object"));
	container_matrix_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("container_diffuse"));
	container_matrix_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("container_specular"));
	container_matrix_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("matrix_emissive"));
	container_matrix_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("container_matrix", container_matrix_mat);

	Material::LoadParams backpack_mat(AssetManager::GetAsset<Shader>("lit_object"));
	backpack_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("backpack_diffuse"));
	backpack_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("backpack_specular"));
	backpack_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	backpack_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("backpack", backpack_mat);

	Material::LoadParams floor_mat(AssetManager::GetAsset<Shader>("lit_object"));
	floor_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("floor_wood_diffuse"));
	floor_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("floor_wood_specular"));
	floor_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	floor_mat.floatParameters.emplace("material.shininess", 32.0f);
	floor_mat.boolParameters.emplace("beta_prevent_tex_scaling", true);
	floor_mat.floatParameters.emplace("beta_tex_scaling_factor", 1.0f);
	AssetManager::LoadAsset<Material>("floor_wood", floor_mat);

	Material::LoadParams flat_emissive_white(AssetManager::GetAsset<Shader>("flat_emissive"));
	flat_emissive_white.vec3Parameters.emplace("emissive", Color::white.toVector());
	AssetManager::LoadAsset<Material>("flat_emissive_white", flat_emissive_white);

	Material::LoadParams flat_emissive_cyan(AssetManager::GetAsset<Shader>("flat_emissive"));
	flat_emissive_cyan.vec3Parameters.emplace("emissive", Color::cyan.toVector());
	AssetManager::LoadAsset<Material>("flat_emissive_cyan", flat_emissive_cyan);


	// Load models 
	AssetManager::LoadAsset<Model>("backpack", Model::FileImportParams{ "backpack/backpack.fbx", { AssetManager::GetAsset<Material>("backpack") } });


	// Load font
	AssetManager::LoadAsset<Font>("octosquares_32", { "octosquares_font/octosquares.ttf", 32, CharacterLoading::ASCII_128 });


	// Load audio
	AssetManager::LoadAsset<AudioSound>("sound", { "vineboom.mp3", ACTIVATE_LOOP });
	AssetManager::LoadAsset<AudioSound>("music", { "TestMusic.mp3", ACTIVATE_3D | ACTIVATE_STREAM | ACTIVATE_LOOP, 50.0f, 100.0f });

}

void ExpositionGame::loadGame()
{
	loadScene(&expositionScene);
}


void ExpositionGame::unloadGame()
{
}