#pragma once
#include <string>
#include <unordered_set>

class GameplayTag
{
public:
	void addGameplayTag(std::string tagToAdd);
	void removeGameplayTag(std::string tagToRemove);
	void clearGameplayTags();

	bool hasGameplayTag(std::string tagToCheck);

private:
	std::unordered_set<std::string> gameplayTags;
};

