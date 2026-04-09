#include "component.h"
#include "entity.h"

Component::Component()
{
}

Component::~Component()
{
}

Component::Component(const Component& other) : owner(other.owner), updateActivated(other.updateActivated)
{
}

Entity* Component::getOwner() const
{
	return owner;
}

void Component::setUpdateActivated(bool updateActive)
{
	updateActivated = updateActive;
}

bool Component::getUdpateActivated() const
{
	return updateActivated;
}

void Component::setOwner(Entity* owner_)
{
	owner = owner_;
}
