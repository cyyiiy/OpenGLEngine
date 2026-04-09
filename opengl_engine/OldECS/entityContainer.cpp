#include "entityContainer.h"
#include <algorithm>

Entity* EntityContainer::createEntity()
{
	entities.push_back(new Entity(*this));
	return entities.back();
}

void EntityContainer::addPendingEntity(Entity* pendingEntity)
{
	pendingEntities.push_back(pendingEntity);
}

void EntityContainer::updateEntities()
{
	for (auto& pending_entity : pendingEntities)
	{
		auto iter = std::find(entities.begin(), entities.end(), pending_entity);
		if (iter != entities.end())
		{
			delete *iter;
			entities.erase(iter);
		}
	}

	pendingEntities.clear();
}

void EntityContainer::clearEntities()
{
	for (auto& entity : entities)
	{
		delete entity;
	}
	entities.clear();
}
