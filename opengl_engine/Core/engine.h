#pragma once
#include <glad/glad.h>

#include "game.h"
#include "window.h"
#include <ECS/entityContainer.h>

#include <string>
#include <memory>


class Engine : private EntityContainer
{
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool initialize(int wndw_width = 1920, int wndw_height = 1080, std::string wndw_name = "OpenGL Engine", bool wndw_capturemouse = true);
	void run();
	void close();

	void loadGame(std::weak_ptr<Game> game_);
	void unloadGame();

	void engineUpdate(GLFWwindow* glWindow);

	void windowResize(GLFWwindow* glWindow, int width, int height);

private:
	std::shared_ptr<Game> game;
	Window window;

	float deltaTime = 0.0f;
	double lastFrame = 0.0f;
};
