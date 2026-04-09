#pragma once
#include <Core/transform.h>
#include <Core/Gameplay/gameplayTag.h>
#include <ECS/component.h>
#include <ECS/componentManager.h>
#include <ServiceLocator/locator.h>

#include <memory>
#include <vector>

class EntityContainer;


class Entity : public Transform, public GameplayTag
{
public:
	Entity(EntityContainer& containerRef_);
	~Entity();

	Entity() = delete;
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	/**
	* Destroy an entity and all of its components.
	* Will take effect at the end of the frame (to avoid issues with the physics and audio updates).
	*/
	void destroyEntity();

	/**
	* Create a component attached to this entity.
	* @return	The created component.
	*/
	template<class T>
	std::enable_if_t<std::is_base_of<Component, T>::value, std::shared_ptr<T>>
		addComponentByClass()
	{
		std::shared_ptr<T> component_created = ComponentManager::CreateComponent<T>(this);
		components.push_back(component_created);
		return component_created;
	}


	/**
	* Get a component of given class in this entity's components.
	* @return	The first component of given class found, nullptr if no component of given class has been found.
	*/
	template<class T>
	std::enable_if_t<std::is_base_of<Component, T>::value, std::shared_ptr<T>>
		getComponentByClass()
	{
		for (std::shared_ptr<Component>& component : components)
		{
			std::shared_ptr<T> component_as_t = std::dynamic_pointer_cast<T>(component);
			if (component_as_t)
			{
				return component_as_t;
			}
		}

		Locator::getLog().LogMessage_Category("Entity: Unable to find a component of given class.", LogCategory::Warning);
		return nullptr;
	}

	/**
	* Get all components of given class in this entity's components.
	* @return	A list of all components of given class found.
	*/
	template<class T>
	std::enable_if_t <std::is_base_of<Component, T>::value, std::vector<std::shared_ptr<T>>>
		getComponentsByClass()
	{
		std::vector<std::shared_ptr<T>> return_list;

		for (std::shared_ptr<Component>& component : components)
		{
			std::shared_ptr<T> component_as_t = std::dynamic_pointer_cast<T>(component);
			if (component_as_t)
			{
				return_list.push_back(component_as_t);
			}
		}

		return return_list;
	}

	/**
	* Get all components of this entity.
	* @return	A list of all components this entity has.
	*/
	std::vector<std::shared_ptr<Component>> getAllComponents()
	{
		return components;
	}

	/**
	* Remove a component of this entity.
	* @param	component	The component to remove.
	*/
	void removeComponent(std::weak_ptr<Component> component);

private:
	std::vector<std::shared_ptr<Component>> components;

	EntityContainer& containerRef;

	void clearComponents();
};
