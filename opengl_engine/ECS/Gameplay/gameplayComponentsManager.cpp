#include "gameplayComponentsManager.h"
#include <ECS/ecs.h>
#include <ECS/entity.h>

#include "lifetimeComponent.h"


void GameplayComponentsManager::Update(float dt)
{
	// Update the lifetime components
	auto& lifetime_comps_manager = ECS::Manager<LifetimeComponent>();
	lifetime_comps_manager.ForEach([dt](LifetimeComponent& lifetime_component)
	{
		lifetime_component.lifetimeTimer -= dt;
		if (lifetime_component.lifetimeTimer <= 0.0f)
		{
			Entity* comp_owner = lifetime_component.getOwner();
			if (comp_owner == nullptr) return;
			comp_owner->destroyEntity();
		}
	});
}
