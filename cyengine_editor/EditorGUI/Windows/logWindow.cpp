#include "editorWindows.h"
#include <EditorUtils/editorStatics.h>
#include <EditorUtils/guiHelper.h>
#include <imgui/imgui.h>


void EditorWindows::ShowLogWindow(const char* layout)
{
	// Set the window flags
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_MenuBar;

	// Create the window
	std::string window_name;
	if (std::string(layout).empty()) window_name = LOGS_WINDOW_NAME;
	else window_name = GuiHelper::WindowLayout(LOGS_WINDOW_NAME, layout);

	ImGui::Begin(window_name.c_str(), NULL, window_flags);

	// Check if the window scroll is already at the bottom (enable auto-scrolling)
	const bool auto_scroll = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f;

	// Set the window menu bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Logs Menu"))
		{
			if (ImGui::MenuItem("Clear Logs")) 
			{
				EditorStatics::logMessages.clear();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Set the window content
	for (LogMessage& log_message : EditorStatics::logMessages)
	{
		ImGui::TextColored(log_message.color, "%s", log_message.log.c_str());
	}

	// Force the scroll to the bottom (make new messages visible) if auto-scrolling is enabled
	if (auto_scroll)
	{
		ImGui::SetScrollHereY(1.0f);
	}

	// End the window section
	ImGui::End();
}