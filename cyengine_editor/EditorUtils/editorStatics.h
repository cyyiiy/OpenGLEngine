#pragma once
#include <Utils/Color.h>
#include <string>
#include <vector>
#include <imgui/imgui.h>

struct LogMessage
{
	std::string log;
	ImVec4 color;
};

class EditorStatics
{
public:
	static bool isViewportFocused;

	static std::vector<LogMessage> logMessages;

	static class Window* window;
};
