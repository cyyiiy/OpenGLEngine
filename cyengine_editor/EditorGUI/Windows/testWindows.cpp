#include "editorWindows.h"
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

void EditorWindows::ShowTestWindowA()
{
	ImGui::Begin("Test Window A", NULL, 0);

	std::string test;
	ImGui::InputText("Test Input", &test);

	ImGui::End();
}