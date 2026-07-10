#include "doomlikeGame.h"
#include <Assets/defaultAssets.h>
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <ECS/componentManager.h>
#include <Inputs/Input.h>
#include <GameplayStatics/gameplayStatics.h>

#include <PrefabFactories/wallFactory.h>
#include <PrefabFactories/floorCeilingFactory.h>
#include <PrefabFactories/stairFactory.h>
#include <PrefabFactories/lampFactory.h>

#include <GameComponents/playerComponent.h>
#include <GameComponents/gunComponent.h>


#ifdef NDEBUG
	const bool DEBUG = false;
#else
	const bool DEBUG = true;
#endif // NDEBUG


void DoomlikeGame::loadGameAssets()
{
	Log& log = Locator::getLog();
	log.SetConsoleLogDisplayRule(LogCategory::Info);

	if (DEBUG) log.LogMessage_Category("Doomlike: Start loading doomlike assets...", LogCategory::Info);
	double load_time = glfwGetTime();
	double full_load_time = load_time;

	DefaultAssets::LoadDefaultAssets();
	if (DEBUG) log.LogMessage_Category("Doomlike: Load default assets time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);


	// Load textures and materials
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

	if (DEBUG) log.LogMessage_Category("Doomlike: Load textures time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
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

	if (DEBUG) log.LogMessage_Category("Doomlike: Load materials time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	// Load models
	AssetManager::LoadModel("taxi", "taxi/taxi.fbx", &AssetManager::GetMaterial("taxi"));
	AssetManager::LoadModel("enemy", "doomlike/enemy/enemy.obj", &AssetManager::GetMaterial("enemy"));
	AssetManager::LoadModel("bullet", "doomlike/bullet/bullet.fbx", &AssetManager::GetMaterial("bullet"));
	AssetManager::LoadModel("gun", "doomlike/gun/gun.obj", &AssetManager::GetMaterial("gun"));

	if (DEBUG) log.LogMessage_Category("Doomlike: Load models time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	// Setup prefab factories
	WallFactory::SetupWallAssets();
	FloorCeilingFactory::SetupFloorCeilingAssets();
	StairFactory::SetupStairAssets();
	LampFactory::SetupLampAssets();

	if (DEBUG) log.LogMessage_Category("Doomlike: Setup prefabs assets time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	// Load sounds
	AssetManager::CreateSound("feet1", "doomlike/sounds/foot_1.mp3", ACTIVATE_3D);
	AssetManager::CreateSound("feet2", "doomlike/sounds/foot_2.mp3", ACTIVATE_3D);
	AssetManager::CreateSound("shoot", "doomlike/sounds/shoot.mp3", 0);
	AudioSound& enemydeath_sound = AssetManager::CreateSound("enemydeath", "doomlike/sounds/enemy_death.mp3", ACTIVATE_3D);
	enemydeath_sound.setMinMaxDistance(1.0f, 20.0f);
	AssetManager::CreateSound("playerdeath", "doomlike/sounds/player_death.mp3", 0);
	AudioSound& elevator_sound = AssetManager::CreateSound("elevator", "doomlike/sounds/elevator.mp3", ACTIVATE_3D | ACTIVATE_LOOP);
	elevator_sound.setMinMaxDistance(1.0f, 10.0f);

	if (DEBUG) log.LogMessage_Category("Doomlike: Load sounds time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	if (DEBUG) log.LogMessage_Category("Doomlike: Finished loading doomlike assets in " + std::to_string(glfwGetTime() - full_load_time) + " seconds.", LogCategory::Info);
}

void DoomlikeGame::loadGame()
{
	Entity* player_entity = createEntity();
	Entity* player_camera_entity = createEntity();
	player = player_entity->addComponentByClass<PlayerComponent>();
	player_entity->addComponentByClass<GunComponent>();
	ECS::GetComponent(player).setupPlayer(player_camera_entity, 1.5f, 5.0f, 7.0f, 0.3f);

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
		Locator::getLog().LogMessage_Category("FPS Demo: Tried to change the level with an index to a level that doesn't exist.", LogCategory::Warning);
		return;
	}

	currentLevel = levelIndex;
	mustRestartLevel = true;
}

void DoomlikeGame::loadLevel(int index)
{
	PlayerComponent& player_comp = ECS::GetComponent(player);

	currentLevel = index;
	switch (index)
	{
	case 0:
		loadScene(&testScene);
		player_comp.respawn(testScene.getSpawnPoint());
		break;
	case 1:
		loadScene(&levelDebugScene);
		player_comp.respawn(levelDebugScene.getSpawnPoint());
		break;
	case 2:
		loadScene(&levelStartScene);
		player_comp.respawn(levelStartScene.getSpawnPoint());
		break;
	case 3:
		loadScene(&levelAdvancedScene);
		player_comp.respawn(levelAdvancedScene.getSpawnPoint());
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