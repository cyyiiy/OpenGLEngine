#include "editorWindows.h"
#include <EditorUtils/editorStatics.h>
#include <ServiceLocator/locator.h>
#include <pfd/portable-file-dialogs.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <filesystem>
#include <fstream>

using EditorTypes::NewProjectData;


bool CreateProject(const NewProjectData& project_data); // Definition below

void EditorWindows::ShowNewProjectWindow()
{
	NewProjectData& new_project_data = EditorStatics::newProjectData;

	// Set the window flags
	const ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking;

	// Force the window size
	ImGui::SetNextWindowSize(ImVec2(550, 200));

	// Create the window
	ImGui::Begin(EditorWindows::NEW_PROJECT_WINDOW, &new_project_data.windowOpened, window_flags);

	// Set the window content
	ImGui::InputText("Project Name", &new_project_data.projectName, ImGuiInputTextFlags_CharsNoBlank);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::InputText("Project Location", &new_project_data.folderPath, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0f, 20.0f);
	if (ImGui::Button("..."))
	{
		const std::string folder = pfd::select_folder("Project Location").result();
		if (!folder.empty())
		{
			new_project_data.folderPath = folder;
		}
	}

	// Empty space to put the "Create Project" button at the bottom of the window
	const float button_height = ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("EmptySpace", ImVec2(0, -button_height));
	ImGui::EndChild();

	ImGui::BeginDisabled(new_project_data.projectName.empty() || new_project_data.folderPath.empty());
	if (ImGui::Button("Create Project", ImVec2(-FLT_MIN, 0.0f)))
	{
		const bool result = CreateProject(new_project_data);
		if (result)
		{
			new_project_data.windowOpened = false;
		}
	}
	ImGui::EndDisabled();

	// End the window section
	ImGui::End();
}

bool CreateProject(const NewProjectData& project_data)
{
	std::filesystem::path source_folder(project_data.folderPath);
	if (!std::filesystem::is_directory(source_folder))
	{
		Locator::getLog().LogMessage_Category("New Project: Selected location is not a valid directory!", LogCategory::Error);
		return false;
	}

	std::filesystem::path project_folder = source_folder / project_data.projectName;
	if (std::filesystem::exists(project_folder))
	{
		Locator::getLog().LogMessage_Category("New Project: A project with this name already exists at the selected location!", LogCategory::Error);
		return false;
	}

	std::error_code error;
	if (!std::filesystem::create_directory(project_folder, error))
	{
		Locator::getLog().LogMessage_Category("New Project: Failed to create the project folder!", LogCategory::Error);
		Locator::getLog().LogMessage_Category(error.message(), LogCategory::Error);
		return false;
	}

	std::filesystem::path project_file = project_folder / project_data.projectName;
	project_file += ".cyproject";
	std::ofstream file(project_file);
	if (!file.is_open())
	{
		Locator::getLog().LogMessage_Category("New Project: Failed to create the project file!", LogCategory::Error);
		return false;
	}

	// TODO: Fill the project file with informations

	file.close();
	Locator::getLog().LogMessage_Category("Successfully created project '" + project_data.projectName + "' at location '" + source_folder.string() + "'.", LogCategory::Info);
	return true;
}