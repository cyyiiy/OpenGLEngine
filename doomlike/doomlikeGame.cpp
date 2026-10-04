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

#include <Rendering/texture.h>
#include <Rendering/material.h>
#include <Rendering/Model/model.h>
#include <Audio/audioSound.h>


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

	AssetManager::LoadAsset<Texture>("crate_diffuse", { "container2.png", false });
	AssetManager::LoadAsset<Texture>("crate_specular", { "container2_specular.png", false });

	AssetManager::LoadAsset<Texture>("enemy_diffuse", { "doomlike/enemy/enemy_basecolor.jpeg", false });
	AssetManager::LoadAsset<Texture>("enemy_specular", { "doomlike/enemy/enemy_roughness.jpeg", false });
	AssetManager::LoadAsset<Texture>("enemy_emissive", { "doomlike/enemy/enemy_emissive.jpeg", false });

	AssetManager::LoadAsset<Texture>("bullet_diffuse", { "doomlike/bullet/bullet_basecolor.png", false });
	AssetManager::LoadAsset<Texture>("bullet_specular", { "doomlike/bullet/bullet_roughness.png", false });
	AssetManager::LoadAsset<Texture>("bullet_emissive", { "doomlike/bullet/bullet_emissive.png", false });

	AssetManager::LoadAsset<Texture>("gun_diffuse", { "doomlike/gun/gun_basecolor.png", false });
	AssetManager::LoadAsset<Texture>("gun_specular", { "doomlike/gun/gun_roughness.png", false });
	AssetManager::LoadAsset<Texture>("gun_emissive", { "doomlike/gun/gun_emissive.png", false });

	AssetManager::LoadAsset<Texture>("hud_crosshair", { "doomlike/hud/crosshair.png", false });

	if (DEBUG) log.LogMessage_Category("Doomlike: Load textures time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();

	Material::LoadParams crate_mat(AssetManager::GetAsset<Shader>("lit_object"));
	crate_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("crate_diffuse"));
	crate_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("crate_specular"));
	crate_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	crate_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("crate", crate_mat);

	Material::LoadParams enemy_mat(AssetManager::GetAsset<Shader>("lit_object"));
	enemy_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("enemy_diffuse"));
	enemy_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("enemy_specular"));
	enemy_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("enemy_emissive"));
	enemy_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("enemy", enemy_mat);

	Material::LoadParams bullet_mat(AssetManager::GetAsset<Shader>("lit_object"));
	bullet_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("bullet_diffuse"));
	bullet_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("bullet_specular"));
	bullet_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("bullet_emissive"));
	bullet_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("bullet", bullet_mat);

	Material::LoadParams gun_mat(AssetManager::GetAsset<Shader>("lit_object"));
	gun_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("gun_diffuse"));
	gun_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("gun_specular"));
	gun_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("gun_emissive"));
	gun_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>("gun", gun_mat);

	if (DEBUG) log.LogMessage_Category("Doomlike: Load materials time: " + std::to_string(glfwGetTime() - load_time), LogCategory::Info);
	load_time = glfwGetTime();


	// Load models
	AssetManager::LoadAsset<Model>("enemy", Model::FileImportParams{ "doomlike/enemy/enemy.obj", { AssetManager::GetAsset<Material>("enemy"), AssetManager::GetAsset<Material>("enemy") } });
	AssetManager::LoadAsset<Model>("bullet", Model::FileImportParams{ "doomlike/bullet/bullet.fbx", { AssetManager::GetAsset<Material>("bullet") } });
	AssetManager::LoadAsset<Model>("gun", Model::FileImportParams{ "doomlike/gun/gun.obj", { AssetManager::GetAsset<Material>("gun"), AssetManager::GetAsset<Material>("gun") } });

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
	AssetManager::LoadAsset<AudioSound>("feet1", { "doomlike/sounds/foot_1.mp3", ACTIVATE_3D });
	AssetManager::LoadAsset<AudioSound>("feet2", { "doomlike/sounds/foot_2.mp3", ACTIVATE_3D });
	AssetManager::LoadAsset<AudioSound>("shoot", { "doomlike/sounds/shoot.mp3" });
	AssetManager::LoadAsset<AudioSound>("enemydeath", { "doomlike/sounds/enemy_death.mp3", ACTIVATE_3D, 1.0f, 20.0f });
	AssetManager::LoadAsset<AudioSound>("playerdeath", { "doomlike/sounds/player_death.mp3", 0 });
	AssetManager::LoadAsset<AudioSound>("elevator", { "doomlike/sounds/elevator.mp3", ACTIVATE_3D | ACTIVATE_LOOP, 1.0f, 10.0f });

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
}