#pragma once

class Scene;


/** Scenes Manager
* Static class dedicated to manage the scenes of the editor.
* Note: Scenes used by this Scenes Manager don't use their `loadAssets` function.
*/
class ScenesManager
{
public:
	/**
	* Load a new active scene. It will replace the currently active scene if there is one.
	* Can also be used to reload the current scene.
	* @param	scene	The scene to load.
	*/
	static void loadScene(Scene* scene);

	/**
	* Unload the currently active scene.
	* To load a new scene, simply call `loadScene` and it will automatically unload the current scene.
	*/
	static void unloadActiveScene();

	/**
	* Update the currently active scene.
	* @param	deltaTime	The delta time of the frame.
	*/
	static void updateActiveScene(float deltaTime);

private:
	static Scene* activeScene;
};

