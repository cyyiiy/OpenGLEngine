#pragma once
#include <ServiceLocator/log.h>
#include <Log/logUtils.h>
#include <unordered_map>


class EditorLogManager : public Log
{
public:
	~EditorLogManager();

	void LogMessageToScreen(const std::string& logText, const Color& logColor, const float logDuration, const std::string logIndex = "") override;
	void EraseLogIndexMessage(const std::string logIndex) override;
	void LogMessage_Category(const std::string& logText, LogCategory logCategory) override;

	void SetScreenLogDisplayRule(LogCategory logCategory) override;
	void SetConsoleLogDisplayRule(LogCategory logCategory) override;


	void Initialize() override;
	void UpdateScreenLogs(float dt) override;


private:
	void printLogToConsole(const std::string& logText, LogCategory logCategory);
	void printLogToLogWindow(const std::string& logText, const Color& logColor);

	std::unordered_map<LogDisplay, LogCategory> logCategoryDisplayRules
	{
		{ LogDisplay::Console, LogCategory::Info },
		{ LogDisplay::Screen, LogCategory::Warning },
		{ LogDisplay::LogFile, LogCategory::Info }
	};
};

