#pragma once
#include <EditorUtils/editorTypes.h>
#include <imgui/imgui.h>
#include <string>
#include <vector>

struct LogMessage
{
	std::string log;
	ImVec4 color;
};

class EditorStatics
{
public:
	static bool isViewportFocused;

	static bool needResetLayout;

	static EditorTypes::NewProjectData newProjectData;

	static std::vector<LogMessage> logMessages;

	static class Window* window;
};
