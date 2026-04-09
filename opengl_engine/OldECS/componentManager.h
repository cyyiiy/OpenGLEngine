#pragma once
#include "component.h"
#include <ServiceLocator/locator.h>

#include <vector>
#include <unordered_map>
#include <memory>

class Entity;


/**
* Component List
* Base class for specialized component lists.
* A component list manage each components of a class and is responsible of storing them continuously in the memory.
*/
class ComponentList
{
public:
	ComponentList(size_t numComponentsPerSublist_, bool updateActivated_) : numComponentsPerSublist(numComponentsPerSublist_), updateActivated(updateActivated_)
	{
	}

	virtual ~ComponentList() {}


	/** Create a component and store it continuously with the other components of this class in the memory. */
	virtual std::shared_ptr<Component> createComponent(Entity* owner) = 0;

	/** Remove a component from the list of this class of components. */
	virtual void deleteComponent(const std::shared_ptr<Component>& component) = 0;

	/** Update the active components of the list. */
	void updateComponents(float deltaTime)
	{
		for (auto& component : componentsShared)
		{
			if (!component->getUdpateActivated()) continue;
			component->update(deltaTime);
		}
	}

	/** Clear the entire component list. Warning: This instantly free the memory of all components, so use this with caution. */
	virtual void clearList() = 0;


	/** Get the maximum number of components of this class that can be stored in a single list. */
	size_t getNumComponentsPerList() const { return numComponentsPerSublist; }

	/** Get the update activated value for the component class of this component list. */
	bool getUpdateActivated() const { return updateActivated; }

	/** Get every components of this class. */
	const std::vector<std::shared_ptr<Component>>& getAllComponents() const { return componentsShared; }


protected:
	/** Initializing a component require to be a friend class of Component, and ComponentListByClass isn't so we do it in the parent class. */
	void initComponent(const std::shared_ptr<Component>& component, Entity* componentOwner);
	
	/** Exiting a component require to be a friend class of Component, and ComponentListByClass isn't so we do it in the parent class. */
	void exitComponent(const std::shared_ptr<Component>& component);

	std::vector<std::shared_ptr<Component>> componentsShared;
	size_t numComponentsPerSublist;
	bool updateActivated;
};



/**
* Component List by Class
* Specialized component list class that manage every components of its defined class.
*/
template<class T>
class ComponentListByClass : public ComponentList
{
private:
	struct ComponentSubList
	{
		std::vector<T> components;
		std::vector<bool> componentUsedBySlot;
		size_t freeSlots;

		ComponentSubList(size_t sublistSize) : freeSlots(sublistSize)
		{
			components.resize(sublistSize);
			componentUsedBySlot.resize(sublistSize);
		}
	};
	std::vector<std::unique_ptr<ComponentSubList>> componentSubLists;

	std::vector<std::weak_ptr<T>> componentsSharedTemplated;


public:
	ComponentListByClass(size_t numComponentsPerSublist_, bool updateActivated_) : ComponentList(numComponentsPerSublist_, updateActivated_)
	{
		std::unique_ptr<ComponentSubList> sublist = std::make_unique<ComponentSubList>(numComponentsPerSublist);
		componentSubLists.push_back(std::move(sublist));
	}

	/** Create a component and store it continuously with the other components of this class in the memory. */
	std::shared_ptr<Component> createComponent(Entity* owner) override
	{
		//  step 1: get a sublist with a free slot available
		//  ------------------------------------------------
		int sublist_index = -1;
		const size_t num_sublists = componentSubLists.size();

		for (size_t sublists_iter = 0; sublists_iter < num_sublists; sublists_iter++)
		{
			if (componentSubLists[sublists_iter]->freeSlots > 0)
			{
				sublist_index = (int)sublists_iter;
				break;
			}
		}

		//  step 1-B: create a new sublist if no free slot
		//  ----------------------------------------------
		if (sublist_index == -1)
		{
			std::unique_ptr<ComponentSubList> sublist = std::make_unique<ComponentSubList>(numComponentsPerSublist);
			componentSubLists.push_back(std::move(sublist));
			sublist_index = (int)num_sublists; //  index of the new sublist will be the total number of sublists before its creation
		}

		ComponentSubList& sublist_creating_in = *componentSubLists[sublist_index];


		//  step 2: "create" the component in the sublist
		//  ---------------------------------------------
		int created_component_slot = -1;
		for (size_t slots_iter = 0; slots_iter < numComponentsPerSublist; slots_iter++)
		{
			if (sublist_creating_in.componentUsedBySlot[slots_iter]) continue;

			//  note: we don't actually create the component (with new) cause it has been done when the sublist was created
			//  instead, we will get a component of the sublist that is unused for now (works like a memory pool)

			sublist_creating_in.componentUsedBySlot[slots_iter] = true;
			sublist_creating_in.freeSlots--;
			created_component_slot = (int)slots_iter;
			break;
		}

		if (created_component_slot == -1)
		{
			Locator::getLog().LogMessage_Category("Component Manager: Failed to find a free memory slot to create a component", LogCategory::Error);
			return std::make_shared<Component>();
		}


		//  step 3: shared_ptr from created component
		//  -----------------------------------------
		ComponentSubList* sublist_ptr = componentSubLists[sublist_index].get(); //  give a raw ptr of the sublist to the destructor lambda

		const std::shared_ptr<T> shared_component_as_t = std::shared_ptr<T>(&sublist_creating_in.components[created_component_slot],
			[this, created_component_slot, sublist_ptr](T* component) //  define a custom destructor for the shared_ptr with a lambda
			{
				//  note: the shared_ptr destructor will be called when its last shared reference will disappear
				//  we use this to free the memory of the component only when we're sure that nobody uses it anymore

				if (componentSubLists.empty()) return; //  security to avoid errors when the game is closing for exemple

				//  we don't actually free the memory since it would mean erasing the entire sublist, but we authorize a new component to replace this one in the sublist (memory pool)
				sublist_ptr->componentUsedBySlot[created_component_slot] = false;
				sublist_ptr->freeSlots++;


				//  checking if we can't delete the sublist
				if (sublist_ptr->freeSlots == numComponentsPerSublist)
				{
					//  the sublist has reached 0 used components, we can delete it
					for (size_t sublists_iter = 0; sublists_iter < numComponentsPerSublist; sublists_iter++)
					{
						//  searching in all sublists the one that reached 0 used components
						if (componentSubLists[sublists_iter].get() == sublist_ptr)
						{
							componentSubLists.erase(componentSubLists.begin() + sublists_iter);
							break;
						}
					}
				}

				//  note: we don't delete the list if it goes empty cause an empty component list cause it's not a bad issue,
				//  except if the game create class of components that are used only once and does this thousands times
				//  all component lists are still destroyed when the game closes
			});

		componentsSharedTemplated.push_back(shared_component_as_t);
		const std::shared_ptr<Component> shared_component = std::dynamic_pointer_cast<Component>(shared_component_as_t);
		componentsShared.push_back(shared_component);


		//  step 4: initialize the created component
		//  ----------------------------------------
		initComponent(shared_component, owner);

		return shared_component;
	}

	/** Remove a component from the list of this class of components. */
	void deleteComponent(const std::shared_ptr<Component>& component) override
	{
		const size_t num_shared_comps = componentsShared.size();
		for (size_t iter_shared_comps = 0; iter_shared_comps < num_shared_comps; iter_shared_comps++)
		{
			if (componentsShared[iter_shared_comps] == component)
			{
				exitComponent(component);
				componentsShared.erase(componentsShared.begin() + iter_shared_comps);
				break;
			}
		}

		std::shared_ptr<T> component_as_t = std::dynamic_pointer_cast<T>(component);
		for (size_t iter_shared_comps = 0; iter_shared_comps < num_shared_comps; iter_shared_comps++)
		{
			if (componentsSharedTemplated[iter_shared_comps].lock() == component_as_t)
			{
				componentsSharedTemplated.erase(componentsSharedTemplated.begin() + iter_shared_comps);
				break;
			}
		}

		//  note: we don't free the memory of the component here for 2 reasons:
		//  1. we don't want to cause errors if other objects still use the component, so we wait for the shared pointer to lose its last reference to continue
		//  2. the sublist system works like a memory pool, so the component will not be deleted, but will be free to be reused
		//  (the memory is freed at the end of the game, or if a sublist reach 0 used components, leading to the suppression of the sublist)
	}

	/** Get all active components in a list of weak ptr to their true class. */
	std::vector<std::weak_ptr<T>>& getAllComponentsTemplated()
	{
		return componentsSharedTemplated;
	}

	/** Clear the entire component list. Warning: This instantly free the memory of all components, so use this with caution. */
	void clearList() override
	{
		componentSubLists.clear();
	}
};



/**
* Component Class Data
* Data associated to a component class to give information of wether or not this class should have an update, and the number of components of this class in a sublist.
*/
struct ComponentClassData
{
	bool updateActivated;
	int numComponentsPerSublist;

	ComponentClassData() : updateActivated(true), numComponentsPerSublist(20) {}
	ComponentClassData(bool updateActivated_, int numComponentsPerSublist_) : updateActivated(updateActivated_), numComponentsPerSublist(numComponentsPerSublist_) {}
};


/**
* Component Manager
* Static class that manage specialized component lists and is the interface to create, update and delete components.
*/
class ComponentManager
{
public:
	/** Create a component list for this component class. Warning: This function does not check if a list for this class already exists. */
	template<typename T>
	static void AddComponentList(size_t classId)
	{
		ComponentClassData component_class_data = GetComponentClassData(classId);

		componentLists[classId] = std::make_unique<ComponentListByClass<T>>(component_class_data.numComponentsPerSublist, component_class_data.updateActivated);
	}

	/** Create a component and return a shared pointer to it. */
	template<typename T>
	static std::shared_ptr<T> CreateComponent(Entity* owner)
	{
		const size_t component_class_id = typeid(T).hash_code(); //  get the "unique id" of the given component class
		if (componentLists.find(component_class_id) == componentLists.end())
		{
			//  if there is no existing list for this component class, create one
			AddComponentList<T>(component_class_id);
		}

		//  create the component in the corresponding component list
		return std::static_pointer_cast<T>(componentLists[component_class_id]->createComponent(owner));
	}

	/** Delete a component. */
	static void DeleteComponent(const std::shared_ptr<Component>& component)
	{
		const size_t component_class_id = typeid(*component.get()).hash_code(); //  get the "unique id" of the class of the component to remove (can't get the id from 'T' cause it will just be the 'Component' class)
		if (componentLists.find(component_class_id) != componentLists.end())
		{
			//  if there is no existing list for this component class, it is impossible to delete the given component
			componentLists[component_class_id]->deleteComponent(component);
		}
	}

	/** Update all components. Note: won't update components of a class that has been registered with disabled update. */
	static void UpdateComponents(float deltaTime)
	{
		for (auto& component_list : componentLists)
		{
			if (!component_list.second->getUpdateActivated()) continue;

			component_list.second->updateComponents(deltaTime);
		}
	}


	/** Get a reference to the component list of the given class. */
	template<typename T>
	static ComponentListByClass<T>& GetComponentListByClass()
	{
		const size_t component_class_id = typeid(T).hash_code(); //  get the "unique id" of the given component class
		return *static_cast<ComponentListByClass<T>*>(componentLists[component_class_id].get());
	}

	/** Get all active components of the given class via a list of weak ptr of their real class. */
	template<typename T>
	static std::vector<std::weak_ptr<T>>& GetAllComponentOfClass()
	{
		return GetComponentListByClass<T>().getAllComponentsTemplated();
	}

	/** Get all active components via a list of shared ptr. */
	static std::vector<std::shared_ptr<Component>> GetAllComponents()
	{
		std::vector<std::shared_ptr<Component>> all_components;
		for (auto& component_list : componentLists)
		{
			const std::vector<std::shared_ptr<Component>>& components = component_list.second->getAllComponents();
			all_components.insert(all_components.end(), components.begin(), components.end());
		}
		return all_components;
	}


	/** Clear the component list of the given class. Warning: This instantly free the memory of the components of the given class, so use with caution. */
	template<typename T>
	static void ClearAllComponentsOfClass()
	{
		const size_t component_class_id = typeid(T).hash_code(); //  get the "unique id" of the given component class
		if (componentLists.find(component_class_id) != componentLists.end())
		{
			//  if there is no existing list for this component class, there is nothing to clear
			componentLists[component_class_id]->clearList();
			componentLists.erase(component_class_id);
		}
	}

	/** Clear all components and all component lists. Warning: This instantly free the memory of every component of the game, so use with caution. */
	static void ClearAllComponents()
	{
		for (auto& component_list : componentLists)
		{
			component_list.second->clearList();
		}
		componentLists.clear();
	}

	/** Register class datas for the given component class. Note that those datas will have effect if there are registered before the first component of this class is created. */
	template<typename T>
	static void RegisterComponentDataByClass(const ComponentClassData& datas)
	{
		const size_t component_class_id = typeid(T).hash_code(); //  get the "unique id" of the given component class
		componentClassDatas[component_class_id] = datas;
	}

	/** Get the registered class datas for a specific component class, or default class datas if there is none. */
	static ComponentClassData GetComponentClassData(size_t classId);


private:
	static std::unordered_map<size_t, std::unique_ptr<ComponentList>> componentLists;
	static std::unordered_map<size_t, ComponentClassData> componentClassDatas;
};

