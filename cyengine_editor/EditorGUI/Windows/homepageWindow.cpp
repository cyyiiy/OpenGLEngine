#include "editorWindows.h"
#include <EditorUtils/guiHelper.h>
#include <imgui/imgui.h>


void EditorWindows::ShowHomepageWindow(const char* layout)
{
	// Set the window flags
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	// Create the window
	std::string window_name;
	if (std::string(layout).empty()) window_name = HOMEPAGE_WINDOW_NAME;
	else window_name = GuiHelper::WindowLayout(HOMEPAGE_WINDOW_NAME, layout);

	ImGui::Begin(window_name.c_str(), NULL, window_flags);

	// Set the window content
	ImGui::Text("Welcome to the Cy-Engine editor");

	// End the window section
	ImGui::End();
}