#include "editorWindows.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

void EditorWindows::ShowTestWindowA()
{
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	ImGui::Begin(TEST_A_WINDOW_NAME, NULL, window_flags);

	std::string test;
	ImGui::InputText("Test Input", &test);

	ImGui::End();
}

void EditorWindows::ShowTestWindowB()
{
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	ImGui::Begin(TEST_B_WINDOW_NAME, NULL, window_flags);

	ImGui::Text("Test Window for layout");

	ImGui::End();
}