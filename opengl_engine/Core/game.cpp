#include "game.h"
#include "scene.h"
#include <GameplayStatics/gameplayStatics.h>


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
	unloadActiveScene(true);
	activeScene = scene;
	GameplayStatics::SetCurrentScene(activeScene);
	activeScene->load();
}

void Game::unloadActiveScene(bool loadNewScene)
{
	if (activeScene) activeScene->unload(!loadNewScene);

	if (!loadNewScene) GameplayStatics::SetCurrentScene(nullptr);
}
