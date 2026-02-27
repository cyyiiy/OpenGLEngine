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

	Renderer& renderer = Locator::getRenderer();

	DefaultAssets::LoadDefaultAssets();

	//  shaders, textures and materials
	AssetManager::CreateShaderProgram("lit_object", "Lit/object_lit.vert", "Lit/object_lit.frag", ShaderType::Lit);

	AssetManager::LoadTexture("container_diffuse", "container2.png", false);
	AssetManager::LoadTexture("container_specular", "container2_specular.png", false);
	AssetManager::LoadTexture("container_emissive", "matrix.jpg", false);

	AssetManager::LoadTexture("backpack_diffuse", "backpack/backpack_basecolor.jpg", false);
	AssetManager::LoadTexture("backpack_specular", "backpack/backpack_specular.jpg", false);

	AssetManager::LoadTexture("smileyface_sprite", "awesomeface.png", false);

	AssetManager::LoadTexture("floor_stone_diffuse", "doomlike/textures/stone_floor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_stone_specular", "doomlike/textures/stone_floor_specular.jpg", false);

	AssetManager::LoadTexture("floor_wood_diffuse", "doomlike/textures/wood_floor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_wood_specular", "doomlike/textures/wood_floor_specular.jpg", false);

	Material& container_mat = AssetManager::CreateMaterial("container", AssetManager::GetShader("lit_object"));
	container_mat.addTexture(&AssetManager::GetTexture("container_diffuse"), TextureType::Diffuse);
	container_mat.addTexture(&AssetManager::GetTexture("container_specular"), TextureType::Specular);
	container_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	container_mat.addParameter("material.shininess", 32.0f);

	Material& backpack_mat = AssetManager::CreateMaterial("backpack", AssetManager::GetShader("lit_object"));
	backpack_mat.addTexture(&AssetManager::GetTexture("backpack_diffuse"), TextureType::Diffuse);
	backpack_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Specular);
	backpack_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	backpack_mat.addParameter("material.shininess", 32.0f);

	Material& floor_mat = AssetManager::CreateMaterial("floor_stone", AssetManager::GetShader("lit_object"));
	floor_mat.addTexture(&AssetManager::GetTexture("floor_stone_diffuse"), TextureType::Diffuse);
	floor_mat.addTexture(&AssetManager::GetTexture("floor_stone_specular"), TextureType::Specular);
	floor_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_mat.addParameter("material.shininess", 32.0f);
	floor_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_mat.addParameter("beta_tex_scaling_factor", 4.0f);

	Material& floor_wood_mat = AssetManager::CreateMaterial("floor_wood", AssetManager::GetShader("lit_object"));
	floor_wood_mat.addTexture(&AssetManager::GetTexture("floor_wood_diffuse"), TextureType::Diffuse);
	floor_wood_mat.addTexture(&AssetManager::GetTexture("floor_wood_specular"), TextureType::Specular);
	floor_wood_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_wood_mat.addParameter("material.shininess", 20.0f);
	floor_wood_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_wood_mat.addParameter("beta_tex_scaling_factor", 4.0f);

	Material& light_source_mat_white = AssetManager::CreateMaterial("light_source_white", AssetManager::GetShader("flat_emissive"));
	light_source_mat_white.addParameter("emissive", Color::white.toVector());

	Material& light_source_mat_cyan = AssetManager::CreateMaterial("light_source_cyan", AssetManager::GetShader("flat_emissive"));
	light_source_mat_cyan.addParameter("emissive", Color::cyan.toVector());


	//  models 
	AssetManager::CreateModel("container").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("container"));

	AssetManager::CreateModel("light_cube_white").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("light_source_white"));
	AssetManager::CreateModel("light_cube_cyan").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("light_source_cyan"));

	AssetManager::LoadMeshCollection("backpack", "backpack/backpack.fbx");
	AssetManager::CreateModel("backpack").addMeshes(AssetManager::GetMeshCollection("backpack"), AssetManager::GetMaterial("backpack"));

	AssetManager::CreateModel("floor_stone");
	AssetManager::GetModel("floor_stone").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor_stone"));

	AssetManager::CreateModel("floor_wood");
	AssetManager::GetModel("floor_wood").addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor_wood"));


	//  audio
	AssetManager::CreateSound("sound", "fartreverb.mp3", ACTIVATE_LOOP);
	AudioSound& music = AssetManager::CreateSound("music", "TestMusic.mp3", ACTIVATE_3D | ACTIVATE_STREAM);
	music.setMinMaxDistance(50.0f, 100.0f);


	//  font
	AssetManager::LoadFont("octosquares_128", "octosquares_font/octosquares.ttf", 128, CharacterLoading::ASCII_128);
}

void ExpositionGame::loadGame()
{
	test_game_entity = createEntity();

	loadScene(&expositionScene);
}


void ExpositionGame::unloadGame()
{
	Renderer& renderer = Locator::getRenderer();

	AssetManager::DeleteMaterial("container");
	AssetManager::DeleteMaterial("light_source_white");
	AssetManager::DeleteMaterial("light_source_cyan");
	AssetManager::DeleteMaterial("backpack");
}