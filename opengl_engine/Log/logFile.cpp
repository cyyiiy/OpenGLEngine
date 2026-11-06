#include "logFile.h"
#include <ServiceLocator/locator.h>

void LogFile::init(const std::string& log_file_name)
{
	logFileStream = std::ofstream(log_file_name);
	if (!logFileStream.is_open())
	{
		Locator::getLog().LogMessage_Category("Log File: Unable to open log file.", LogCategory::Error);
	}
}

void LogFile::exit()
{
	if (!logFileStream.is_open())
	{
		Locator::getLog().LogMessage_Category("Log File: Unable to close log file.", LogCategory::Error);
		return;
	}
	logFileStream.close();
}

void LogFile::addMessage(const std::string& message)
{
	if (!logFileStream.is_open())
	{
		Locator::getLog().LogMessage_Category("Log File: Unable to write in log file.", LogCategory::Error);
		return;
	}
	logFileStream << message << std::endl;
}
