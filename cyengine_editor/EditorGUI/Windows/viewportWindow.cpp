#include "editorWindows.h"
#include <imgui/imgui.h>

void EditorWindows::ShowViewportWindow()
{
	// Set the window flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// Note: Window position/size can be forced here, docking too?

	// Create the window
	ImGui::Begin("Viewport", NULL, window_flags);

	// Set the window content
	ImGui::Text("TODO: Render the viewport texture");

	// End the window section
	ImGui::End();
}