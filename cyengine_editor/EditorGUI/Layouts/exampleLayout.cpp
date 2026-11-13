#include "editorLayouts.h"
#include <EditorGUI/Windows/editorWindows.h>
#include <EditorUtils/editorStatics.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


void ResetExampleLayout(ImGuiID dockspaceId, ImVec2 layoutSize); // Definition below

void EditorLayouts::ShowExampleLayout()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	EditorWindows::ShowMainMenu();


	// Setup the host window of the layout
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	const ImGuiWindowFlags layout_window_flags =
		ImGuiWindowFlags_NoTitleBar            |
		ImGuiWindowFlags_NoCollapse            |
		ImGuiWindowFlags_NoResize              |
		ImGuiWindowFlags_NoMove                |
		ImGuiWindowFlags_NoDocking             |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus            |
		ImGuiWindowFlags_NoBackground
	;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("LayoutWindow", NULL, layout_window_flags);
	ImGui::PopStyleVar(3);


	// We want to reset the layout if its entry is not found in the .ini file
	ImGuiID dockspace_id = ImGui::GetID("LayoutDockSpace");
	const bool no_ini = (ImGui::DockBuilderGetNode(dockspace_id) == nullptr);

	// Create the dock space (the layout support)
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	if (no_ini || EditorStatics::needResetLayout)
	{
		EditorStatics::needResetLayout = false;
		ResetExampleLayout(dockspace_id, viewport->WorkSize);
	}


	ImGui::End(); // Close the layout window


	// Show the windows of the example layout
	EditorWindows::ShowTestWindowA();
	EditorWindows::ShowTestWindowB();
	EditorWindows::ShowLogWindow();
	EditorWindows::ShowViewportWindow();
	// Note: The order matters here, as for windows with tabs it will show the last created one at application launch
	// Also, the very last window created is the one focused at application launch
}

void ResetExampleLayout(ImGuiID dockspaceId, ImVec2 layoutSize)
{
	ImGui::DockBuilderRemoveNode(dockspaceId);
	ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceId, layoutSize);

	// First slice, 25% from the right of the layout
	ImGuiID id_right, id_other;
	id_right = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.25f, nullptr, &id_other);

	// Second slice, 80% from the top of the "unsliced" part
	ImGuiID id_top, id_bottom;
	id_top = ImGui::DockBuilderSplitNode(id_other, ImGuiDir_Up, 0.80f, nullptr, &id_bottom);

	// Assign the windows to the layout
	ImGui::DockBuilderDockWindow(EditorWindows::VIEWPORT_WINDOW_NAME, id_top);
	ImGui::DockBuilderDockWindow(EditorWindows::LOGS_WINDOW_NAME, id_bottom);
	ImGui::DockBuilderDockWindow(EditorWindows::TEST_A_WINDOW_NAME, id_right);
	ImGui::DockBuilderDockWindow(EditorWindows::TEST_B_WINDOW_NAME, id_bottom); // Node re-usage create tabs
	
	ImGui::DockBuilderFinish(dockspaceId);
}