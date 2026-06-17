#include "logManager.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <ECS/entity.h>
#include <iostream>
#include <algorithm>


LogManager::~LogManager()
{
	logFileWriter.exit();
	logMessagesOnScreen.clear();
}


void LogManager::LogMessageToScreen(const std::string& logText, const Color& logColor, const float logDuration, const std::string logIndex)
{
	printLogToConsole(logText, LogCategory::Custom);
	displayLogToScreen(logText, LogCategory::Custom, logColor, logDuration, logIndex);
	writeLogToFile(logText, LogCategory::Custom);
}

void LogManager::EraseLogIndexMessage(const std::string logIndex)
{
	for (auto& screen_log : logMessagesOnScreen)
	{
		if (screen_log.hasSameIndex(logIndex))
		{
			screen_log.timer = 0.0f; // Mark for delete on next update
			break;
		}
	}
}

void LogManager::LogMessage_Category(const std::string& logText, LogCategory logCategory)
{
	printLogToConsole(logText, logCategory);
	displayLogToScreen(logText, logCategory, LogCategoryToColor(logCategory), 5.0f, "");
	writeLogToFile(logText, logCategory);
}

void LogManager::SetScreenLogDisplayRule(LogCategory logCategory)
{
	logCategoryDisplayRules[LogDisplay::Screen] = logCategory;
}

void LogManager::SetConsoleLogDisplayRule(LogCategory logCategory)
{
	logCategoryDisplayRules[LogDisplay::Console] = logCategory;
}


void LogManager::Initialize()
{
	logFileWriter.init();
}

void LogManager::UpdateScreenLogs(float dt)
{
	std::vector<LogMessageScreen> expired_logs;

	for (auto& screen_log : logMessagesOnScreen)
	{
		screen_log.timer -= dt;
		if (screen_log.timer <= 0.0f) expired_logs.push_back(screen_log);
	}

	if (expired_logs.size() <= 0) return;

	for (auto& expired_log : expired_logs)
	{
		ECS::DeleteComponent(expired_log.text);
		auto iter = std::find(logMessagesOnScreen.begin(), logMessagesOnScreen.end(), expired_log);
		logMessagesOnScreen.erase(iter);
	}
	logMessagesOnScreen.shrink_to_fit();
	expired_logs.clear();

	float next_y_offset = 0.0f;
	for (auto& screen_log : logMessagesOnScreen)
	{
		screen_log.yOffset = next_y_offset;
		TextComponent& screen_log_text_comp = ECS::GetComponent(screen_log.text);
		screen_log_text_comp.position.offset = SCREEN_LOG_BASE_OFFSET + Vector2{ 0.0f, screen_log.yOffset };
		next_y_offset -= (screen_log_text_comp.getTextSize().y + SCREEN_NEW_LOG_OFFSET);
	}
}


void LogManager::printLogToConsole(const std::string& logText, LogCategory logCategory)
{
	if (logCategory < logCategoryDisplayRules[LogDisplay::Console]) return;

	std::cout << LogCategoryToString(logCategory) << logText << std::endl;
}

void LogManager::displayLogToScreen(const std::string& logText, LogCategory logCategory, const Color& logColor, const float logDuration, const std::string logIndex)
{
	if (logCategory < logCategoryDisplayRules[LogDisplay::Screen]) return;

	for (auto& screen_log : logMessagesOnScreen)
	{
		if (screen_log.hasSameIndex(logIndex))
		{
			TextComponent& screen_log_text_comp = ECS::GetComponent(screen_log.text);
			screen_log_text_comp.setText(logText);
			screen_log_text_comp.tintColor = logColor;
			screen_log.timer = logDuration;
			return;
		}
	}

	// Screen log has not overriden another screen log, create new one
	float y_offset = 0.0f;
	if (logMessagesOnScreen.size() >= 1)
	{
		LogMessageScreen& screen_log = logMessagesOnScreen.at(logMessagesOnScreen.size() - 1);
		y_offset = screen_log.yOffset - ECS::GetComponent(screen_log.text).getTextSize().y - SCREEN_NEW_LOG_OFFSET;
	}

	logMessagesOnScreen.emplace_back(LogMessageScreen
		(
			logIndex,
			ECS::CreateComponent<TextComponent>(),
			logDuration,
			y_offset
		)
	);

	TextComponent& text_comp = ECS::GetComponent(logMessagesOnScreen.at(logMessagesOnScreen.size() - 1).text);
	text_comp.setTextDatas(logText, AssetManager::GetFont("arial_24"));
	text_comp.position = HudPosition{ Vector2{ 0.0f, 1.0f }, Vector2{ 0.0f, 1.0f }, SCREEN_LOG_BASE_OFFSET + Vector2{ 0.0f, y_offset } };
	text_comp.scale = Vector2{ 0.6f };
	text_comp.tintColor = logColor;
}

void LogManager::writeLogToFile(const std::string& logText, LogCategory logCategory)
{
	if (logCategory < logCategoryDisplayRules[LogDisplay::LogFile]) return;

	logFileWriter.addMessage(LogCategoryToString(logCategory) + logText);
}
