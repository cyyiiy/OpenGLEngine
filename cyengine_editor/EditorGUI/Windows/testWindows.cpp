#include "editorWindows.h"
#include <EditorUtils/guiHelper.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

void EditorWindows::ShowTestWindowA(const char* layout)
{
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	std::string window_name;
	if (std::string(layout).empty()) window_name = TEST_A_WINDOW_NAME;
	else window_name = GuiHelper::WindowLayout(TEST_A_WINDOW_NAME, layout);

	ImGui::Begin(window_name.c_str(), NULL, window_flags);

	std::string test;
	ImGui::InputText("Test Input", &test);

	ImGui::End();
}

void EditorWindows::ShowTestWindowB(const char* layout)
{
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	std::string window_name;
	if (std::string(layout).empty()) window_name = TEST_B_WINDOW_NAME;
	else window_name = GuiHelper::WindowLayout(TEST_B_WINDOW_NAME, layout);

	ImGui::Begin(window_name.c_str(), NULL, window_flags);

	ImGui::Text("Test Window for layout");

	ImGui::End();
}