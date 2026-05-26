#include "physicsManager.h"
#include "boxCollisionComponent.h"
#include "collisionsAABB.h"
#include <ECS/ecs.h>
#include <Rendering/Shapes/shapePoint.h>
#include <Rendering/Shapes/shapeLine.h>
#include <Rendering/Shapes/shapeCube.h>
#include <algorithm>


bool PhysicsManager::LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene)
{
	// 1. Initialize values
	outHitInfos = RaycastHitInfos();
	bool hit = false;

	// 2. Create pointers from these values so they can be passed in the lambda in read/write
	RaycastHitInfos* infos_ptr = &outHitInfos;
	bool* hit_ptr = &hit;

	// 3. Initialize the ray for the computation
	Ray ray;
	ray.setupWithStartEnd(start, end);

	// 4. Test all collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([this, ray, testChannels, infos_ptr, hit_ptr](const BoxCollisionComponent& box_collision_component)
	{
		const bool col_hit = this->resolveLineRaycast(box_collision_component, ray, testChannels, *infos_ptr);
		*hit_ptr = *hit_ptr || col_hit;
	});

	// 5. Call the event on the collision hit by the raycast
	if (ECS::IsComponentHandleValid(outHitInfos.hitCollision))
	{
		ECS::GetComponent(outHitInfos.hitCollision).onRaycastIntersect.broadcast(RaycastType::RaycastTypeLine, outHitInfos.hitLocation);
	}


	if (drawDebugTime == 0.0f) return hit; // Don't create the raycast renderer if no draw debug

	// 6. Create the raycast renderer component
	ComponentHandle<RaycastRendererComponent> raycast = ECS::CreateComponent<RaycastRendererComponent>();
	if (createOnScene) sceneRaycasts.push_back(raycast);
	
	// 7. Initialize values of the raycast renderer component
	RaycastRendererComponent& raycast_comp = ECS::GetComponent(raycast);
	raycast_comp.lifetime = drawDebugTime;
	raycast_comp.registeredOnScene = createOnScene;

	// 8. Create the shapes of the raycast renderer
	if (hit)
	{
		raycast_comp.shapes = {
			std::make_shared<ShapeLine>(start, outHitInfos.hitLocation, Color::green),
			std::make_shared<ShapePoint>(outHitInfos.hitLocation, Color::green),
			std::make_shared<ShapeLine>(outHitInfos.hitLocation, end, Color::red)
		};
	}
	else
	{
		raycast_comp.shapes = { std::make_shared<ShapeLine>(start, end, Color::green) };
	}

	return hit;
}


void PhysicsManager::UpdatePhysics(float dt)
{
	// Reset 'debugIntersectedLastFrame' parameter on box collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([](BoxCollisionComponent box_collision_component)
	{
		box_collision_component.debugIntersectedLastFrame = false;
	});


	// Update raycast renderer components lifetimes
	auto& raycast_renderers_manager = ECS::Manager<RaycastRendererComponent>();
	raycast_renderers_manager.ForEach([this, dt](RaycastRendererComponent& raycast_renderer_component)
	{
		// Raycast with a negative lifetime stay indefinitely
		if (raycast_renderer_component.lifetime < 0.0f) return;

		raycast_renderer_component.lifetime -= dt;
		if (raycast_renderer_component.lifetime <= 0.0f)
		{
			ComponentHandle<RaycastRendererComponent> handle = raycast_renderer_component.getSelfHandle<RaycastRendererComponent>();

			if (raycast_renderer_component.registeredOnScene)
			{
				// Remove the raycast component from the scene raycasts list
				auto iter = std::find(this->sceneRaycasts.begin(), this->sceneRaycasts.end(), handle);
				if (iter != this->sceneRaycasts.end()) sceneRaycasts.erase(iter);
			}

			ECS::DeleteComponent(handle);
		}
	});
}

void PhysicsManager::ClearRaycastOnSceneUnload(bool exitGame)
{
	if (exitGame)
	{
		auto& raycast_renderers_manager = ECS::Manager<RaycastRendererComponent>();
		raycast_renderers_manager.ClearAllComponents(false);
		sceneRaycasts.clear();
		return;
	}


	for (auto& raycast_renderer_handle : sceneRaycasts)
	{
		if (ECS::IsComponentHandleValid(raycast_renderer_handle))
		{
			ECS::DeleteComponent(raycast_renderer_handle);
		}
	}

	sceneRaycasts.clear();
}


bool PhysicsManager::channelTest(const std::string& collisionChannel, const std::vector<std::string>& testChannels)
{
	if (testChannels.empty()) return true; // Empty test channels = test everything

	for (const std::string& test_channel : testChannels)
	{
		if (test_channel == collisionChannel) return true;
	}

	return false;
}

bool PhysicsManager::resolveLineRaycast(const BoxCollisionComponent& boxColComponent, const Ray& ray, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos)
{
	// 1. Check if the collision component can be tested by the raycast
	if (!channelTest(boxColComponent.collisionChannel, testChannels)) return false;

	// 2. Compute the line raycast intersection
	return CollisionsAABB::IntersectLineRaycast(boxColComponent, ray, outHitInfos);
}
