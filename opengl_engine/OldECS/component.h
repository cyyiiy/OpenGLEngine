#pragma once
#include <memory>

class Entity;

class Component
{
public:
	Component();
	~Component();
	Component(const Component& other); //  we need the copy constructor for the vector

	Component& operator=(const Component&) = delete;

	/** Get the Entity that owns this component. */
	Entity* getOwner() const;

	void setUpdateActivated(bool updateActive);
	bool getUdpateActivated() const;

protected:
	friend class Entity;
	friend class ComponentList;
	friend class ComponentManager;

	virtual void registerComponent() {}
	virtual void unregisterComponent() {}

	/** Called after the component has been created, but before it is registered. */
	virtual void init() {}

	/** Called each frame. Note: won't be called if the class of this component has been registered with update disabled. */
	virtual void update(float deltaTime) {}

	/** Called before the component is removed, but after it is unregistered. */
	virtual void exit() {}

private:
	Entity* owner{ nullptr };
	bool updateActivated{ true };

	void setOwner(Entity* owner_);
};

