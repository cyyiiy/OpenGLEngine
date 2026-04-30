#pragma once

#include <glad/glad.h>

#include "game.h"
#include "window.h"
#include <Rendering/rendererOpenGL.h>
#include <ECS/entityContainer.h>
#include <Rendering/cameraComponent.h>
//#include <Rendering/Text/textRendererComponent.h>

//#include <Audio/audioManager.h>
//#include <Audio/audioSound.h>

//#include <Log/logManager.h>

#include <Maths/matrix4.h>
#include <Maths/vector3.h>

#include <string>
#include <memory>


class Engine : private EntityContainer
{
public:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool initialize(int wndw_width = 1920, int wndw_height = 1080, std::string wndw_name = "OpenGL Engine", bool wndw_capturemouse = true);
	void run();
	void close();

	void loadGame(std::weak_ptr<Game> game_);
	void unloadGame();

	void engineUpdate(GLFWwindow* glWindow);

	//  window resize function redirected
	void windowResize(GLFWwindow* glWindow, int width, int height);

private:
	//  game
	std::shared_ptr<Game> game;

	//  window
	Window window;

	//  renderer
	RendererOpenGL* renderer{ nullptr };

	//  audio manager
	//AudioManager* audio{ nullptr };

	//  log manager
	//LogManager* log{ nullptr };

	//  cameras
	Entity* debugCamEntity{ nullptr };
	ComponentHandle<CameraComponent> debugCamera;
	float debugCameraSpeed{ 4.0f };
	float debugCameraMouseSensitivity{ 0.1f };

	//  debug text
	//std::shared_ptr<TextRendererComponent> fpsText{ nullptr };
	int frameCounter = 0;
	float frameTimeCounter = 0.0f;

	//  time
	float deltaTime = 0.0f;
	double lastFrame = 0.0f;

	//  pause, freecam and debug view
	bool gamePaused{ false };
	bool oneFrame{ false };
	bool freecamMode{ false };
	bool debugViewMode{ false };
	void pauseGame();
	void unpauseGame();
	void advanceOneFrame();
	void enableFreecam();
	void disableFreecam();
	void enableDebugView();
	void disableDebugView();
};
