#pragma once
#include <vector>
#include <string>
#include <fstream>

class LogFile
{
public:
	void init(const std::string& log_file_name);
	void exit();
	void addMessage(const std::string& message);

private:
	std::ofstream logFileStream;
};