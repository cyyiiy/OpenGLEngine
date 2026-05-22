#pragma once
#include <string>
#include <unordered_map>
#include <vector>


const std::string defaultEverything = "default_everything";


class CollisionChannels
{
public:
	static void RegisterTestChannel(std::string name, std::vector<std::string> testChannel);

	static std::vector<std::string> GetRegisteredTestChannel(std::string name);

	static std::string DefaultEverything() { return defaultEverything; }

private:
	static std::unordered_map<std::string, std::vector<std::string>> registeredTestsChannels;
};