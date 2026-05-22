#include "collisionChannels.h"
#include <ServiceLocator/locator.h>

std::unordered_map<std::string, std::vector<std::string>> CollisionChannels::registeredTestsChannels;

void CollisionChannels::RegisterTestChannel(std::string name, std::vector<std::string> testChannel)
{
	if (registeredTestsChannels.find(name) != registeredTestsChannels.end())
	{
		Locator::getLog().LogMessage_Category("Collision Channels: Tried to register a test channel with a name that already exists. Name is " + name + ".", LogCategory::Error);
		return;
	}

	registeredTestsChannels.emplace(name, testChannel);
}

std::vector<std::string> CollisionChannels::GetRegisteredTestChannel(std::string name)
{
	if (registeredTestsChannels.find(name) == registeredTestsChannels.end())
	{
		Locator::getLog().LogMessage_Category("Collision Channels: Tried to get a registered test channel with a name that doesn't exists. Name is " + name + ".", LogCategory::Error);
		return { "" };
	}

	return registeredTestsChannels.at(name);
}