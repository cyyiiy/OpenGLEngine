#pragma once
#include "entity.h"
#include <vector>

class EntityContainer
{
public:
	virtual ~EntityContainer() {}

	/** Creates an Entity that will be registered on the Scene/Game this function has been called on. */
	Entity* createEntity();

	/** Add an entity to the pending entity list for it to be destroyed at the end of the frame. */
	void addPendingEntity(Entity* pendingEntity);
	
protected:
	std::vector<Entity*> entities;
	std::vector<Entity*> pendingEntities;

	/**
	* Update pending destroyed entites at the end of the update tick.
	* Do not call this, it's an engine function.
	*/
	void updateEntities();

	/**
	* Clear all entites registered by this object when exiting a Scene or closing the Game.
	* Do not call this, it's an engine function.
	*/
	void clearEntities();
};

