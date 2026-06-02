#include "expositionGame.h"
#include <ServiceLocator/locator.h>
#include <Assets/defaultAssets.h>
#include <Assets/assetManager.h>
#include <Inputs/Input.h>

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

	// Load shaders, textures and materials
	AssetManager::CreateShaderProgram("lit_object", "Lit/object_lit.vert", "Lit/object_lit.frag", ShaderType::Lit);

	AssetManager::LoadTexture("container_diffuse", "container2.png", false);
	AssetManager::LoadTexture("container_specular", "container2_specular.png", false);

	AssetManager::LoadTexture("backpack_diffuse", "backpack/backpack_basecolor.jpg", false);
	AssetManager::LoadTexture("backpack_specular", "backpack/backpack_specular.jpg", false);

	AssetManager::LoadTexture("floor_wood_diffuse", "doomlike/textures/wood_floor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_wood_specular", "doomlike/textures/wood_floor_specular.jpg", false);

	AssetManager::LoadTexture("smileyface_sprite", "awesomeface.png", false);

	Material& container_mat = AssetManager::CreateMaterial("container", AssetManager::GetShader("lit_object"));
	container_mat.addTexture(&AssetManager::GetTexture("container_diffuse"), TextureType::Diffuse);
	container_mat.addTexture(&AssetManager::GetTexture("container_specular"), TextureType::Specular);
	container_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	container_mat.addParameter("material.shininess", 32.0f);

	Material& backpack_mat = AssetManager::CreateMaterial("backpack", AssetManager::GetShader("lit_object"));
	backpack_mat.addTexture(&AssetManager::GetTexture("backpack_diffuse"), TextureType::Diffuse);
	backpack_mat.addTexture(&AssetManager::GetTexture("backpack_specular"), TextureType::Specular);
	backpack_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	backpack_mat.addParameter("material.shininess", 32.0f);

	Material& floor_mat = AssetManager::CreateMaterial("floor_wood", AssetManager::GetShader("lit_object"));
	floor_mat.addTexture(&AssetManager::GetTexture("floor_wood_diffuse"), TextureType::Diffuse);
	floor_mat.addTexture(&AssetManager::GetTexture("floor_wood_specular"), TextureType::Specular);
	floor_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_mat.addParameter("material.shininess", 32.0f);
	floor_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_mat.addParameter("beta_tex_scaling_factor", 1.0f);

	Material& flat_emissive_white = AssetManager::CreateMaterial("flat_emissive_white", AssetManager::GetShader("flat_emissive"));
	flat_emissive_white.addParameter("emissive", Color::white.toVector());

	Material& flat_emissive_cyan = AssetManager::CreateMaterial("flat_emissive_cyan", AssetManager::GetShader("flat_emissive"));
	flat_emissive_cyan.addParameter("emissive", Color::cyan.toVector());


	// Load models 
	AssetManager::CreateModel("container").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("container"));
	AssetManager::LoadMeshCollection("backpack", "backpack/backpack.fbx");
	AssetManager::CreateModel("backpack").addMeshes(AssetManager::GetMeshCollection("backpack"), AssetManager::GetMaterial("backpack"));

	AssetManager::CreateModel("floor_wood").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor_wood"));

	AssetManager::CreateModel("white_emissive_cube").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("flat_emissive_white"));
	AssetManager::CreateModel("cyan_emissive_cube").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("flat_emissive_cyan"));


	// Load font
	AssetManager::LoadFont("octosquares_32", "octosquares_font/octosquares.ttf", 32, CharacterLoading::ASCII_128);


	// Load audio
	AssetManager::CreateSound("sound", "vineboom.mp3", ACTIVATE_LOOP);
	AudioSound& music = AssetManager::CreateSound("music", "TestMusic.mp3", ACTIVATE_3D | ACTIVATE_STREAM | ACTIVATE_LOOP);
	music.setMinMaxDistance(50.0f, 100.0f);
}

void ExpositionGame::loadGame()
{
	loadScene(&expositionScene);
}


void ExpositionGame::unloadGame()
{
}