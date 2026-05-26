#include "scene.h"
#include <ServiceLocator/locator.h>

void Scene::load() 
{
	firstFrame = true;

	loadScene();
}

void Scene::unload(bool exitGame)
{
	unloadScene();

	Locator::getPhysics().ClearRaycastOnSceneUnload(exitGame);

	clearEntities();
}

void Scene::update(float dt)
{
	if (firstFrame)
	{
		firstFrame = false;
		return;
	}

	updateScene(dt);
}

void Scene::lateUpdate()
{
	updateEntities();
}
