#include "doomlikeGame.h"
#include <Assets/defaultAssets.h>
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <ECS/componentManager.h>
#include <Physics/ObjectChannels/collisionChannels.h>
#include <Inputs/Input.h>
#include <GameplayStatics/gameplayStatics.h>

#include <PrefabFactories/wallFactory.h>
#include <PrefabFactories/floorCeilingFactory.h>
#include <PrefabFactories/stairFactory.h>
#include <PrefabFactories/lampFactory.h>

#include <GameComponents/OverrideComponents/cameraLagComponent.h>
#include <GameComponents/playerComponent.h>
#include <GameComponents/gunComponent.h>
#include <GameComponents/bulletComponent.h>
#include <GameComponents/movingPlatformComponent.h>
#include <GameComponents/targetComponent.h>
#include <GameComponents/enemyComponent.h>


void DoomlikeGame::loadGameAssets()
{
	Log& log = Locator::getLog();
	log.SetConsoleLogDisplayRule(LogCategory::Info);

	log.LogMessage_Category("Doomlike: Start loading doomlike assets...", LogCategory::Info);
	double load_time = glfwGetTime();
	double full_load_time = load_time;

	ComponentManager::RegisterComponentDataByClass<CameraLagComponent>(ComponentClassData{ true, 1 });
	ComponentManager::RegisterComponentDataByClass<PlayerComponent>(ComponentClassData{ true, 1 });
	ComponentManager::RegisterComponentDataByClass<GunComponent>(ComponentClassData{ true, 1 });
	ComponentManager::RegisterComponentDataByClass<MovingPlatformComponent>(ComponentClassData{ true, 10 });
	ComponentManager::RegisterComponentDataByClass<TargetComponent>(ComponentClassData{ false, 20 });
	ComponentManager::RegisterComponentDataByClass<BulletComponent>(ComponentClassData{ true, 20 });
	ComponentManager::RegisterComponentDataByClass<EnemyComponent>(ComponentClassData{ true, 10 });

	DefaultAssets::LoadDefaultAssets();


	//  shaders, textures and materials
	AssetManager::CreateShaderProgram("lit_object", "Lit/object_lit.vert", "Lit/object_lit.frag", ShaderType::Lit);

	log.LogMessage_Category("Doomlike: Load default assets time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();

	AssetManager::LoadTexture("crate_diffuse", "container2.png", false);
	AssetManager::LoadTexture("crate_specular", "container2_specular.png", false);

	AssetManager::LoadTexture("taxi_diffuse", "taxi/taxi_basecolor.png", false);
	AssetManager::LoadTexture("taxi_emissive", "taxi/taxi_emissive.png", false);

	AssetManager::LoadTexture("enemy_diffuse", "doomlike/enemy/enemy_basecolor.jpeg", false);
	AssetManager::LoadTexture("enemy_specular", "doomlike/enemy/enemy_roughness.jpeg", false);
	AssetManager::LoadTexture("enemy_emissive", "doomlike/enemy/enemy_emissive.jpeg", false);

	AssetManager::LoadTexture("bullet_diffuse", "doomlike/bullet/bullet_basecolor.png", false);
	AssetManager::LoadTexture("bullet_specular", "doomlike/bullet/bullet_roughness.png", false);
	AssetManager::LoadTexture("bullet_emissive", "doomlike/bullet/bullet_emissive.png", false);

	AssetManager::LoadTexture("gun_diffuse", "doomlike/gun/gun_basecolor.png", false);
	AssetManager::LoadTexture("gun_specular", "doomlike/gun/gun_roughness.png", false);
	AssetManager::LoadTexture("gun_emissive", "doomlike/gun/gun_emissive.png", false);

	AssetManager::LoadTexture("hud_crosshair", "doomlike/hud/crosshair.png", false);

	log.LogMessage_Category("Doomlike: Load textures time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();

	Material& crate_mat = AssetManager::CreateMaterial("crate", AssetManager::GetShader("lit_object"));
	crate_mat.addTexture(&AssetManager::GetTexture("crate_diffuse"), TextureType::Diffuse);
	crate_mat.addTexture(&AssetManager::GetTexture("crate_specular"), TextureType::Specular);
	crate_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	crate_mat.addParameter("material.shininess", 32.0f);

	Material& taxi_mat = AssetManager::CreateMaterial("taxi", AssetManager::GetShader("lit_object"));
	taxi_mat.addTexture(&AssetManager::GetTexture("taxi_diffuse"), TextureType::Diffuse);
	taxi_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Specular);
	taxi_mat.addTexture(&AssetManager::GetTexture("taxi_emissive"), TextureType::Emissive);
	taxi_mat.addParameter("material.shininess", 32.0f);

	Material& enemy_mat = AssetManager::CreateMaterial("enemy", AssetManager::GetShader("lit_object"));
	enemy_mat.addTexture(&AssetManager::GetTexture("enemy_diffuse"), TextureType::Diffuse);
	enemy_mat.addTexture(&AssetManager::GetTexture("enemy_specular"), TextureType::Specular);
	enemy_mat.addTexture(&AssetManager::GetTexture("enemy_emissive"), TextureType::Emissive);
	enemy_mat.addParameter("material.shininess", 32.0f);

	Material& bullet_mat = AssetManager::CreateMaterial("bullet", AssetManager::GetShader("lit_object"));
	bullet_mat.addTexture(&AssetManager::GetTexture("bullet_diffuse"), TextureType::Diffuse);
	bullet_mat.addTexture(&AssetManager::GetTexture("bullet_specular"), TextureType::Specular);
	bullet_mat.addTexture(&AssetManager::GetTexture("bullet_emissive"), TextureType::Emissive);
	bullet_mat.addParameter("material.shininess", 32.0f);

	Material& gun_mat = AssetManager::CreateMaterial("gun", AssetManager::GetShader("lit_object"));
	gun_mat.addTexture(&AssetManager::GetTexture("gun_diffuse"), TextureType::Diffuse);
	gun_mat.addTexture(&AssetManager::GetTexture("gun_specular"), TextureType::Specular);
	gun_mat.addTexture(&AssetManager::GetTexture("gun_emissive"), TextureType::Emissive);
	gun_mat.addParameter("material.shininess", 32.0f);

	log.LogMessage_Category("Doomlike: Load materials time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	//  meshes and models
	AssetManager::LoadMeshCollection("taxi", "taxi/taxi.fbx");
	AssetManager::LoadMeshCollection("enemy", "doomlike/enemy/enemy.obj");
	AssetManager::LoadMeshCollection("bullet", "doomlike/bullet/bullet.fbx");
	AssetManager::LoadMeshCollection("gun", "doomlike/gun/gun.obj");

	AssetManager::CreateModel("crate");
	AssetManager::GetModel("crate").addMesh(AssetManager::GetSingleMesh("default_cube"), AssetManager::GetMaterial("crate"));

	AssetManager::CreateModel("taxi");
	AssetManager::GetModel("taxi").addMeshes(AssetManager::GetMeshCollection("taxi"), AssetManager::GetMaterial("taxi"));

	AssetManager::CreateModel("enemy");
	AssetManager::GetModel("enemy").addMeshes(AssetManager::GetMeshCollection("enemy"), AssetManager::GetMaterial("enemy"));

	AssetManager::CreateModel("bullet");
	AssetManager::GetModel("bullet").addMeshes(AssetManager::GetMeshCollection("bullet"), AssetManager::GetMaterial("bullet"));

	AssetManager::CreateModel("gun");
	AssetManager::GetModel("gun").addMeshes(AssetManager::GetMeshCollection("gun"), AssetManager::GetMaterial("gun"));

	log.LogMessage_Category("Doomlike: Load meshes & create models time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	//  decor setups
	WallFactory::SetupWallAssets();
	FloorCeilingFactory::SetupFloorCeilingAssets();
	StairFactory::SetupStairAssets();
	LampFactory::SetupLampAssets();

	log.LogMessage_Category("Doomlike: Setup prefabs assets time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	//  sounds
	AssetManager::CreateSound("feet1", "doomlike/sounds/foot_1.mp3", ACTIVATE_3D);
	AssetManager::CreateSound("feet2", "doomlike/sounds/foot_2.mp3", ACTIVATE_3D);
	AssetManager::CreateSound("shoot", "doomlike/sounds/shoot.mp3", 0);
	AudioSound& enemydeath_sound = AssetManager::CreateSound("enemydeath", "doomlike/sounds/enemy_death.mp3", ACTIVATE_3D);
	enemydeath_sound.setMinMaxDistance(1.0f, 20.0f);
	AssetManager::CreateSound("playerdeath", "doomlike/sounds/player_death.mp3", 0);
	AudioSound& elevator_sound = AssetManager::CreateSound("elevator", "doomlike/sounds/elevator.mp3", ACTIVATE_3D | ACTIVATE_LOOP);
	elevator_sound.setMinMaxDistance(1.0f, 10.0f);

	log.LogMessage_Category("Doomlike: Load sounds time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	//  object channels
	CollisionChannels::RegisterTestChannel("PlayerEntity", { "solid", "enemy", "trigger_zone" }); //  for player and player bullets
	CollisionChannels::RegisterTestChannel("Enemy", { "solid", "player", "bullet" });


	log.LogMessage_Category("Doomlike: Finished loading doomlike assets in " + std::to_string(glfwGetTime() - full_load_time) + " seconds.", LogCategory::Info);
}

void DoomlikeGame::loadGame()
{
	Entity* player_entity = createEntity();
	player = player_entity->addComponentByClass<PlayerComponent>();
	player_entity->addComponentByClass<GunComponent>();
	player->setupPlayer(1.5f, 5.0f, 7.0f, 0.3f);

	loadLevel(2);
}


void DoomlikeGame::updateGame(float dt)
{
	if (mustRestartLevel)
	{
		loadLevel(currentLevel);
		mustRestartLevel = false;
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_0))
	{
		loadLevel(0);
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_1))
	{
		loadLevel(1);
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_2))
	{
		loadLevel(2);
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_3))
	{
		loadLevel(3);
	}
}

void DoomlikeGame::restartLevel()
{
	mustRestartLevel = true;
}

void DoomlikeGame::changeLevel(int levelIndex)
{
	if (levelIndex < 0 || levelIndex > 3)
	{
		Locator::getLog().LogMessage_Category("Doomlike: Tried to change the level with an index to a level that doesn't exist.", LogCategory::Warning);
		return;
	}

	currentLevel = levelIndex;
	mustRestartLevel = true;
}

void DoomlikeGame::loadLevel(int index)
{
	currentLevel = index;
	switch (index)
	{
	case 0:
		loadScene(&testScene);
		player->respawn(testScene.getSpawnPoint());
		break;
	case 1:
		loadScene(&levelDebugScene);
		player->respawn(levelDebugScene.getSpawnPoint());
		break;
	case 2:
		loadScene(&levelStartScene);
		player->respawn(levelStartScene.getSpawnPoint());
		break;
	case 3:
		loadScene(&levelAdvancedScene);
		player->respawn(levelAdvancedScene.getSpawnPoint());
		break;
	}
}


void DoomlikeGame::unloadGame()
{
	AssetManager::DeleteMaterial("crate");
	AssetManager::DeleteMaterial("taxi");
	AssetManager::DeleteMaterial("gun");
	AssetManager::DeleteMaterial("enemy");
	AssetManager::DeleteMaterial("bullet");

	WallFactory::ReleaseWallAssets();
	FloorCeilingFactory::ReleaseFloorCeilingAssets();
	StairFactory::ReleaseStairAssets();
	LampFactory::ReleaseLampAssets();
}