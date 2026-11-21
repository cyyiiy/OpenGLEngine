#include "editorLayouts.h"
#include <EditorGUI/Windows/editorWindows.h>
#include <EditorUtils/editorStatics.h>
#include <EditorUtils/guiHelper.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


void ResetHomepageLayout(ImGuiID dockspaceId, ImVec2 layoutSize); // Definition below

void EditorLayouts::ShowHomepageLayout()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	EditorWindows::ShowMainMenu();


	// Setup the host window of the layout
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	const ImGuiWindowFlags layout_window_flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground
		;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(GuiHelper::CombineChars(HOMEPAGE_LAYOUT_NAME, "Window").c_str(), NULL, layout_window_flags);
	ImGui::PopStyleVar(3);


	// We want to reset the layout if its entry is not found in the .ini file
	ImGuiID dockspace_id = ImGui::GetID(GuiHelper::CombineChars(HOMEPAGE_LAYOUT_NAME, "Dockspace").c_str());
	const bool no_ini = (ImGui::DockBuilderGetNode(dockspace_id) == nullptr);

	// Create the dock space (the layout support)
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	if (no_ini || EditorStatics::needResetLayout)
	{
		EditorStatics::needResetLayout = false;
		ResetHomepageLayout(dockspace_id, viewport->WorkSize);
	}


	ImGui::End(); // Close the layout window


	// Show the windows of the homepage layout
	EditorWindows::ShowHomepageWindow(HOMEPAGE_LAYOUT_NAME);
	EditorWindows::ShowLogWindow(HOMEPAGE_LAYOUT_NAME);
}

void ResetHomepageLayout(ImGuiID dockspaceId, ImVec2 layoutSize)
{
	ImGui::DockBuilderRemoveNode(dockspaceId);
	ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceId, layoutSize);

	// First slice, 20% from the bottom of the layout
	ImGuiID id_top, id_bottom;
	id_bottom = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Down, 0.20f, nullptr, &id_top);

	// Assign the windows to the layout
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::HOMEPAGE_WINDOW_NAME, EditorLayouts::HOMEPAGE_LAYOUT_NAME).c_str(), id_top);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::LOGS_WINDOW_NAME, EditorLayouts::HOMEPAGE_LAYOUT_NAME).c_str(), id_bottom);

	ImGui::DockBuilderFinish(dockspaceId);
}