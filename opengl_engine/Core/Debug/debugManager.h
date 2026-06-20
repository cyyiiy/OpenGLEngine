#pragma once
#include <ECS/ecsTypes.h>
#include <string>


namespace DebugConsts
{
	const float FREECAM_SLOW_SPEED = 4.0f;
	const float FREECAM_FAST_SPEED = 10.0f;
	const float FREECAM_MOUSE_SENSITIVITY = 0.1f;
}


/** Debug Manager
* Static class dedicated to debug systems of the engine.
*/
class DebugManager
{
public:
	/**
	* Initialize the debug manager (create the debug components).
	* Automatically called when setting up the engine.
	* @param	entityOwner		The entity container where the free camera will be created.
	*/
	static void InitializeDebugManager(class EntityContainer& entityOwner);

	/**
	* Update the debug manager (process default inputs and freecam movement).
	* Automatically called every frame by the engine.
	* @param	dt	The delta time of the current frame.
	*/
	static void UpdateDebugManager(float dt);


	/** Pause the game (ECS update, physics, etc. won't be called during the pause). */
	static void PauseGame();

	/** Unpause the game. Also disable the freecam if it was enabled. */
	static void UnpauseGame();

	/** Return true if the game is currently paused. */
	static bool GetPauseState() noexcept;


	/** Unpause the game for only one frame. Only works if the game is already paused. */
	static void AdvanceOneFrame();

	/** Return true if the one frame advance has been required and consume it. */
	static bool ConsumeOneFrameAdvance() noexcept;


	/** Enable the free camera. Also pause the game if currently unpaused. */
	static void EnableFreecam();

	/** Disable the free camera. */
	static void DisableFreecam();

	/** Return true if the freecam is currently enabled. */
	static bool GetFreecamState() noexcept;


	/** Enable the debug view mode (collision view, fps info, etc.). */
	static void EnableDebugView();

	/** Disable the debug view mode. */
	static void DisableDebugView();

	/** Return true if the debug view is currently enabled. */
	static bool GetDebugViewState() noexcept;

private:
	static bool pause;
	static bool oneFrame;
	static bool freecam;
	static float freecamSpeed;
	static bool debugView;

	static int fpsCounter;
	static float fpsTimeCounter;
	static std::string currentFpsText;

	static class Entity* freeCameraEntity;
	static ComponentHandle<class CameraComponent> freeCamera;
	static ComponentHandle<class TextComponent> debugInfoText;

	static void ProcessDebugInputs();
	static void ProcessFreecamInputs(float dt);
	static void UpdateDebugInfoText(float dt);
};