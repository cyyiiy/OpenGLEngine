#pragma once
#include <ServiceLocator/log.h>
#include "logUtils.h"
#include "logFile.h"
#include <ECS/ecs.h>
#include <Rendering/Text/textComponent.h>
#include <Maths/Vector2.h>
#include <unordered_map>
#include <vector>
#include <memory>

class Entity;

struct LogMessageScreen
{
	std::string index;
	ComponentHandle<TextComponent> text;
	float timer;
	float yOffset;

	LogMessageScreen()
	{
		index = "";
		text = ComponentHandle<TextComponent>(); // Initialize invalid (empty) component handle 
		timer = 0.0f;
		yOffset = 0.0f;
	}

	LogMessageScreen(const std::string& index_, const ComponentHandle<TextComponent> text_, const float timer_, const float yOffset_)
	{
		index = index_;
		text = text_;
		timer = timer_;
		yOffset = yOffset_;
	}

	LogMessageScreen(const LogMessageScreen& other)
	{
		index = other.index;
		text = other.text;
		timer = other.timer;
		yOffset = other.yOffset;
	}

	LogMessageScreen& operator=(const LogMessageScreen& other)
	{
		index = other.index;
		text = other.text;
		timer = other.timer;
		yOffset = other.yOffset;
		return *this;
	}

	bool operator==(const LogMessageScreen& other) const
	{
		return text == other.text;
	}

	operator bool() const
	{
		return ECS::IsComponentHandleValid(text);
	}

	bool hasSameIndex(const std::string otherIndex) const
	{
		return index != "" && index == otherIndex;
	}
};


const Vector2 SCREEN_LOG_BASE_OFFSET{ 20.0f, -20.0f };
const float SCREEN_NEW_LOG_OFFSET{ 10.0f };


/**
* The log service provider class.
*/
class LogManager : public Log
{
public:
	~LogManager();

	void LogMessageToScreen(const std::string& logText, const Color& logColor, const float logDuration, const std::string logIndex = "") override;
	void EraseLogIndexMessage(const std::string logIndex) override;
	void LogMessage_Category(const std::string& logText, LogCategory logCategory) override;

	void SetScreenLogDisplayRule(LogCategory logCategory) override;
	void SetConsoleLogDisplayRule(LogCategory logCategory) override;


	// For the engine class (which is the only class that have access to the full log manager)
	void initialize();
	void updateScreenLogs(float dt);


private:
	void printLogToConsole(const std::string& logText, LogCategory logCategory);
	void displayLogToScreen(const std::string& logText, LogCategory logCategory, const Color& logColor, const float logDuration, const std::string logIndex);
	void writeLogToFile(const std::string& logText, LogCategory logCategory);

	std::unordered_map<LogDisplay, LogCategory> logCategoryDisplayRules
	{
		{ LogDisplay::Console, LogCategory::Warning },
		{ LogDisplay::Screen, LogCategory::Custom },
		{ LogDisplay::LogFile, LogCategory::Info }
	};

	std::vector<LogMessageScreen> logMessagesOnScreen;

	LogFile logFileWriter;
};