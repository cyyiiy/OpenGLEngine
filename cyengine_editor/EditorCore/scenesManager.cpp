#include "scenesManager.h"
#include <Core/scene.h>

Scene* ScenesManager::activeScene;


void ScenesManager::loadScene(Scene* scene)
{
	unloadActiveScene();

	activeScene = scene;
	activeScene->load();
}

void ScenesManager::unloadActiveScene()
{
	if (!activeScene) return;

	// Using parameter "exitGame" to true so all raycasts get cleared
	activeScene->unload(true);
}