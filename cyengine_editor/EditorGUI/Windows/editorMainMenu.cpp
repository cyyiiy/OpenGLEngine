#include "editorWindows.h"
#include <EditorUtils/editorStatics.h>
#include <ServiceLocator/locator.h>
#include <Core/window.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>


void EditorWindows::ShowMainMenu()
{
	// Note: To have a main menu with clickable icons, it needs to be a window with specific parameters

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				EditorTypes::NewProjectData& new_project_data = EditorStatics::newProjectData;
				if (!new_project_data.windowOpened)
				{
					new_project_data.windowOpened = true;
					new_project_data.projectName = "";
					new_project_data.folderPath = "";
				}
			}

			if (ImGui::MenuItem("Open Project"))
			{
				// TODO: Open an existing project
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				Locator::getLog().LogMessage_Category("Save menu button clicked, saving...", LogCategory::Info);
				// TODO: Add a save command here
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				Window& window_ref = *EditorStatics::window;
				glfwSetWindowShouldClose(window_ref.getGLFWwindow(), true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Restore layout"))
			{
				Locator::getLog().LogMessage_Category("Reset window layout to default.", LogCategory::Info);
				EditorStatics::needResetLayout = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}