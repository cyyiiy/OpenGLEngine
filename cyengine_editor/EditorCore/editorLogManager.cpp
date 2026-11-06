#include "editorLogManager.h"
#include <EditorUtils/editorStatics.h>
#include <iostream>


EditorLogManager::~EditorLogManager()
{
}

void EditorLogManager::LogMessageToScreen(const std::string& logText, const Color& logColor, const float logDuration, const std::string logIndex)
{
	printLogToConsole(logText, LogCategory::Custom);
	printLogToLogWindow(logText, logColor);
}

void EditorLogManager::EraseLogIndexMessage(const std::string logIndex)
{
}

void EditorLogManager::LogMessage_Category(const std::string& logText, LogCategory logCategory)
{
	printLogToConsole(logText, logCategory);
	printLogToLogWindow(LogCategoryToString(logCategory) + logText, LogCategoryToColor(logCategory));
}

void EditorLogManager::SetScreenLogDisplayRule(LogCategory logCategory)
{
	logCategoryDisplayRules[LogDisplay::Screen] = logCategory;
}

void EditorLogManager::SetConsoleLogDisplayRule(LogCategory logCategory)
{
	logCategoryDisplayRules[LogDisplay::Console] = logCategory;
}

void EditorLogManager::Initialize()
{
}

void EditorLogManager::UpdateScreenLogs(float dt)
{
}

void EditorLogManager::printLogToConsole(const std::string& logText, LogCategory logCategory)
{
	if (logCategory < logCategoryDisplayRules[LogDisplay::Console]) return;

	std::cout << LogCategoryToString(logCategory) << logText << std::endl;
}

void EditorLogManager::printLogToLogWindow(const std::string& logText, const Color& logColor)
{
	const ImVec4 color{ logColor.r / 255.0f, logColor.g / 255.0f, logColor.b / 255.0f, logColor.a / 255.0f };
	EditorStatics::logMessages.push_back(LogMessage{ logText, color });
}
