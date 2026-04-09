#include "entity.h"
#include "entityContainer.h"
#include <algorithm>

Entity::Entity(EntityContainer& containerRef_) : Transform(), containerRef(containerRef_)
{
}

Entity::~Entity()
{
	clearComponents();
}

void Entity::destroyEntity()
{
	//  at the end of the frame, the container this entity is on will delete this entity
	containerRef.addPendingEntity(this);
}

void Entity::removeComponent(std::weak_ptr<Component> component)
{
	std::shared_ptr<Component> component_shared = component.lock();

	auto iter = std::find(components.begin(), components.end(), component_shared);
	if (iter != components.end())
	{
		ComponentManager::DeleteComponent(component_shared);
		components.erase(iter);
	}
}

void Entity::clearComponents()
{
	for (auto& component : components)
	{
		ComponentManager::DeleteComponent(component);
	}
	components.clear();
}