#pragma once
#include <Core/game.h>
#include <Core/scene.h>
#include <Maths/Vector2Int.h>
#include <Events/event.h>

class Engine;

class GameplayStatics
{
public:
	/*
	* Get the currently active Game.
	*/
	static Game* GetGame();

	/*
	* Get the currently active Scene.
	*/
	static Scene* GetActiveScene();

	/*
	* Get the size of the Window.
	*/
	static Vector2Int GetWindowSize();

	/*
	* Get the delta time of the current frame.
	*/
	static float GetDeltaTime();

	/*
	* Get the engine time of the current frame.
	* Engine time is the delta time minus the opengl buffer swap.
	*/
	static float GetEngineTime();


	/* Event called when the screen (window) is resized. */
	static Event<const Vector2Int> OnScrenResize;


private:
	static Game* currentGame;
	static Scene* currentScene;
	static Vector2Int windowSize;

	static float deltaTime;
	static float engineTime;


	friend class Engine;
	friend class Game;

	static void SetCurrentGame(Game* game);
	static void SetCurrentScene(Scene* scene);
	static void SetWindowSize(const Vector2Int& size);

	static void SetDeltaTime(float time);
	static void SetEngineTime(float time);
};

