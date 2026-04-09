#include "componentManager.h"
#include "entity.h"

//  static variables need to be defined in a cpp file to exist for the compiler
std::unordered_map<size_t, std::unique_ptr<ComponentList>> ComponentManager::componentLists;
std::unordered_map<size_t, ComponentClassData> ComponentManager::componentClassDatas;


void ComponentList::initComponent(const std::shared_ptr<Component>& component, Entity* componentOwner)
{
	component->setOwner(componentOwner);
	component->setUpdateActivated(true); //  since the component manager is a memory pool, reset this value to default
	component->init();
	component->registerComponent();
}

void ComponentList::exitComponent(const std::shared_ptr<Component>& component)
{
	component->unregisterComponent();
	component->exit();
}


ComponentClassData ComponentManager::GetComponentClassData(size_t classId)
{
	if (componentClassDatas.find(classId) == componentClassDatas.end())
	{
		return ComponentClassData();
	}

	return componentClassDatas[classId];
}
