#pragma once
#include <stdint.h>
#include <string>
#include <Utils/Color.h>

enum class LogCategory : uint8_t
{
	Info = 0, // Logs that provide informations
	Warning = 1, // Logs that serve for non-critical problems
	Error = 2, // Logs that are used for critical errors
	Custom = 3, // Logs that need to not follow the category display rules
	Crash = 255
};

enum class LogDisplay : uint8_t
{
	Console, // Display logs on the Visual Studio console
	Screen, // Display logs directly on the game window
	LogFile // Display logs in the generated log file
};


static std::string LogCategoryToString(LogCategory logCategory)
{
	switch (logCategory)
	{
	case LogCategory::Info:
		return "Info: ";

	case LogCategory::Warning:
		return "Warning: ";

	case LogCategory::Error:
		return "Error: ";

	case LogCategory::Crash:
		return "Fatal Error: ";

	default:
		return "";
	}
}

static Color LogCategoryToColor(LogCategory logCategory)
{
	switch (logCategory)
	{
	case LogCategory::Info:
		return Color::white;

	case LogCategory::Warning:
		return Color::yellow;

	case LogCategory::Error:
		return Color::red;

	case LogCategory::Crash:
		return Color::red;

	default:
		return Color::white;
	}
}