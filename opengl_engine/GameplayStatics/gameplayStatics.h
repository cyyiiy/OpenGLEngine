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

	/**
	* Draw a debug point for a desired duration.
	* @param	pointPosition	The position in world-space of the point to draw.
	* @param	color			The color of the point to draw.
	* @param	duration		The duration of the debug draw (in seconds).
	*/
	static void DrawDebugPoint(const struct Vector3& pointPosition, const struct Color& color, float duration);

	/**
	* Draw a debug line between two points for a desired duration.
	* @param	pointA		The position in world-space of the first extremity of the line.
	* @param	pointB		The position in world-space of the second extremity of the line.
	* @param	color		The color of the line to draw.
	* @param	duration	The duration of the debug draw (in seconds).
	*/
	static void DrawDebugLine(const struct Vector3& pointA, const struct Vector3& pointB, const struct Color& color, float duration);

	/*
	* Draw a debug cube for a desired duration.
	* @param	boxInfos	The box values of the cube to draw.
	* @param	color		The color of the cube to draw.
	* @param	duration	The duration of the debug draw (in seconds).
	*/
	static void DrawDebugCube(const class Box& boxInfos, const struct Color& color, float duration);


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

	static void UpdateDebugs(float deltaTime);
};

