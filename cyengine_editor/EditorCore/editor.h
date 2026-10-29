#pragma once
#include <glad/glad.h>
#include <Core/window.h>
#include <ECS/entityContainer.h>
#include <string>


class Editor : private EntityContainer
{
public:
	Editor() = default;
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;

	bool initialize(int wndw_width = 1920, int wndw_height = 1080);
	void run();
	void close();

	void windowResize(GLFWwindow* glWindow, int width, int height);

private:
	Window window;
};