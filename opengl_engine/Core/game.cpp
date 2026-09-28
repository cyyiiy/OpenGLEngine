#include "game.h"
#include "scene.h"
#include <GameplayStatics/gameplayStatics.h>
#include <Assets/assetManager.h>


void Game::load()
{
	loadGameAssets();
	loadGame();
}

void Game::unload()
{
	unloadActiveScene(false);

	unloadGame();

	clearEntities();
}

void Game::update(float dt)
{
	updateGame(dt);

	if (activeScene) activeScene->update(dt);
}

bool Game::hasActiveScene()
{
	if (activeScene) return true;
	return false;
}

void Game::lateUpdate()
{
	if (activeScene) activeScene->lateUpdate();
	updateEntities();
}

void Game::loadScene(Scene* scene)
{
	// Unload old scene objects
	const std::string old_scene_name = activeScene ? activeScene->getSceneName() : "";
	unloadActiveScene(true);

	// Load new scene assets
	const std::string new_scene_name = scene->getSceneName();
	if (new_scene_name != "")
	{
		AssetManager::OpenLoadingGroup(new_scene_name);
		scene->loadSceneAssets();
		AssetManager::CloseLoadingGroup();
	}

	// Load new scene objects
	activeScene = scene;
	GameplayStatics::SetCurrentScene(activeScene);
	activeScene->load();

	// Unload old scene assets
	if (old_scene_name != "")
	{
		AssetManager::TryUnloadAssetsOfGroup(old_scene_name);
	}
}

void Game::unloadActiveScene(bool loadNewScene)
{
	if (activeScene) activeScene->unload(!loadNewScene);

	if (!loadNewScene)
	{
		AssetManager::TryUnloadAssetsOfGroup(activeScene->getSceneName());
		GameplayStatics::SetCurrentScene(nullptr);
	}
}
