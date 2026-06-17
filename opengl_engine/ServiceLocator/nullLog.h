#pragma once
#include <ServiceLocator/log.h>

/**
* The log null service provider class.
*/
class NullLog : public Log
{
public:
	void LogMessageToScreen(const std::string& logText, const Color& logColor, const float logDuration, const std::string logIndex = "") override {}
	void EraseLogIndexMessage(const std::string logIndex) override {}
	void LogMessage_Category(const std::string& logText, LogCategory logCategory) override {}

	void SetScreenLogDisplayRule(LogCategory logCategory) override {}
	void SetConsoleLogDisplayRule(LogCategory logCategory) override {}


	void Initialize() override {}
	void UpdateScreenLogs(float dt) override {}
};