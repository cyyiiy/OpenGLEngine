#include "editorLayouts.h"
#include <EditorGUI/Windows/editorWindows.h>
#include <EditorUtils/editorStatics.h>
#include <EditorUtils/guiHelper.h>
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
	ImGui::Begin(GuiHelper::CombineChars(EXAMPLE_LAYOUT_NAME, "Window").c_str(), NULL, layout_window_flags);
	ImGui::PopStyleVar(3);


	// We want to reset the layout if its entry is not found in the .ini file
	ImGuiID dockspace_id = ImGui::GetID(GuiHelper::CombineChars(EXAMPLE_LAYOUT_NAME, "Dockspace").c_str());
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
	EditorWindows::ShowViewportWindow(EXAMPLE_LAYOUT_NAME);
	EditorWindows::ShowLogWindow(EXAMPLE_LAYOUT_NAME);
	EditorWindows::ShowTestWindowA(EXAMPLE_LAYOUT_NAME);
	EditorWindows::ShowTestWindowB(EXAMPLE_LAYOUT_NAME);
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
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::VIEWPORT_WINDOW_NAME, EditorLayouts::EXAMPLE_LAYOUT_NAME).c_str(), id_top);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::LOGS_WINDOW_NAME, EditorLayouts::EXAMPLE_LAYOUT_NAME).c_str(), id_bottom);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::TEST_A_WINDOW_NAME, EditorLayouts::EXAMPLE_LAYOUT_NAME).c_str(), id_right);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::TEST_B_WINDOW_NAME, EditorLayouts::EXAMPLE_LAYOUT_NAME).c_str(), id_bottom); // Node re-usage create tabs
	
	ImGui::DockBuilderFinish(dockspaceId);
}


// -----


void ResetAlternativeLayout(ImGuiID dockspaceId, ImVec2 layoutSize); // Definition below

void EditorLayouts::ShowAlternativeLayout()
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
	ImGui::Begin(GuiHelper::CombineChars(ALTERNATIVE_LAYOUT_NAME, "Window").c_str(), NULL, layout_window_flags);
	ImGui::PopStyleVar(3);


	// We want to reset the layout if its entry is not found in the .ini file
	ImGuiID dockspace_id = ImGui::GetID(GuiHelper::CombineChars(ALTERNATIVE_LAYOUT_NAME, "Dockspace").c_str());
	const bool no_ini = (ImGui::DockBuilderGetNode(dockspace_id) == nullptr);

	// Create the dock space (the layout support)
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	if (no_ini || EditorStatics::needResetLayout)
	{
		EditorStatics::needResetLayout = false;
		ResetAlternativeLayout(dockspace_id, viewport->WorkSize);
	}


	ImGui::End(); // Close the layout window


	// Show the windows of the example layout
	EditorWindows::ShowTestWindowB(ALTERNATIVE_LAYOUT_NAME);
	EditorWindows::ShowLogWindow(ALTERNATIVE_LAYOUT_NAME);
	EditorWindows::ShowViewportWindow(ALTERNATIVE_LAYOUT_NAME);
}

void ResetAlternativeLayout(ImGuiID dockspaceId, ImVec2 layoutSize)
{
	ImGui::DockBuilderRemoveNode(dockspaceId);
	ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceId, layoutSize);

	// First slice, 30% from the left of the layout
	ImGuiID id_left, id_other;
	id_left = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.30f, nullptr, &id_other);

	// Second slide, 60% from the top of the "unsliced" part
	ImGuiID id_top, id_bottom;
	id_top = ImGui::DockBuilderSplitNode(id_other, ImGuiDir_Up, 0.60f, nullptr, &id_bottom);

	// Assign the windows to the layout
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::VIEWPORT_WINDOW_NAME, EditorLayouts::ALTERNATIVE_LAYOUT_NAME).c_str(), id_top);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::LOGS_WINDOW_NAME, EditorLayouts::ALTERNATIVE_LAYOUT_NAME).c_str(), id_bottom);
	ImGui::DockBuilderDockWindow(GuiHelper::WindowLayout(EditorWindows::TEST_B_WINDOW_NAME, EditorLayouts::ALTERNATIVE_LAYOUT_NAME).c_str(), id_left);

	ImGui::DockBuilderFinish(dockspaceId);
}