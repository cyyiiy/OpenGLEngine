#pragma once

namespace EditorWindows
{
	const static char* NEW_PROJECT_WINDOW = "New Project";
	const static char* HOMEPAGE_WINDOW_NAME = "Homepage";
	const static char* VIEWPORT_WINDOW_NAME = "Viewport";
	const static char* LOGS_WINDOW_NAME = "Logs";
	const static char* TEST_A_WINDOW_NAME = "Test Window A";
	const static char* TEST_B_WINDOW_NAME = "Test Window B";


	void ShowMainMenu();

	void ShowNewProjectWindow();

	void ShowHomepageWindow(const char* layout);

	void ShowViewportWindow(const char* layout);

	void ShowLogWindow(const char* layout);

	void ShowTestWindowA(const char* layout);
	void ShowTestWindowB(const char* layout);
}