#include "editorWindows.h"
#include <imgui/imgui.h>
#include <iostream>


void EditorWindows::ShowLogWindow()
{
	// Set the window flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_MenuBar;

	// Create the window
	ImGui::Begin("Logs", NULL, window_flags);

	// Set the window menu bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Logs Menu"))
		{
			if (ImGui::MenuItem("Clear Logs")) 
			{
				std::cout << "Clearing logs...\n";
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Set the window content
	for (int i = 0; i < 100; i++)
	{
		ImGui::Text("Example log %i", i);
	}

	// End the window section
	ImGui::End();
}