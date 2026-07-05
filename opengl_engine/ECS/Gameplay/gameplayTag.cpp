#include "gameplayTag.h"

void GameplayTag::addGameplayTag(std::string tagToAdd)
{
	gameplayTags.insert(tagToAdd);
}

void GameplayTag::removeGameplayTag(std::string tagToRemove)
{
	gameplayTags.erase(tagToRemove);
}

void GameplayTag::clearGameplayTags()
{
	gameplayTags.clear();
}

bool GameplayTag::hasGameplayTag(std::string tagToCheck)
{
	return gameplayTags.count(tagToCheck) > 0;
}
