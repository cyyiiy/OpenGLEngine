#include "entity.h"
#include "entityContainer.h"

Entity::Entity(EntityContainer& containerRef_) : Transform(), containerRef(containerRef_)
{
}

void Entity::clearAllComponents(bool instantDestroy)
{
    // 'pair' is: std::pair<ComponentTypeId, std::vector<StoredComponent>>
    for (auto& pair : components)
    {
        // Loop through all components of a class
        for (StoredComponent& stored_component : pair.second)
        {
            stored_component.deleteFunction(stored_component.raw_handle, instantDestroy);
        }
    }
    
    components.clear();
}

void Entity::destroyEntity()
{
    containerRef.addPendingEntity(this);
}