#include "physicsManager.h"
#include "boxCollisionComponent.h"
#include "rigidbodyComponent.h"
#include "collisionsAABB.h"
#include "collisionTests.h"

#include <ECS/ecs.h>
#include <ServiceLocator/locator.h>
#include <Rendering/Shapes/shapePoint.h>
#include <Rendering/Shapes/shapeLine.h>
#include <Rendering/Shapes/shapeCube.h>
#include <algorithm>


bool PhysicsManager::LineRaycast(const Vector3& start, const Vector3& end, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene)
{
	if (enableInfoLog) Locator::getLog().LogMessage_Category("Physics: Create a line raycast.", LogCategory::Info);

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

bool PhysicsManager::AABBRaycast(const Vector3& location, const Box& aabbBox, const std::vector<std::string> testChannels, float drawDebugTime, bool createOnScene)
{
	if (enableInfoLog) Locator::getLog().LogMessage_Category("Physics: Create an AABB raycast.", LogCategory::Info);

	// 1. Initialize values
	bool hit = false;
	std::vector<ComponentHandle<BoxCollisionComponent>> intersected_cols;

	// 2. Create pointers from these values so they can be passed in the lambda in read/write
	bool* hit_ptr = &hit;
	std::vector<ComponentHandle<BoxCollisionComponent>>* intersected_cols_ptr = &intersected_cols;

	// 3. Initialize the box for the computation
	Box box = aabbBox;
	box.setCenterPoint(box.getCenterPoint() + location);

	// 4. Test all collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([this, box, testChannels, hit_ptr, intersected_cols_ptr](const BoxCollisionComponent& box_collision_component)
	{
		const bool col_hit = this->resolveAABBRaycast(box_collision_component, box, testChannels, *intersected_cols_ptr);
		*hit_ptr = *hit_ptr || col_hit;
	});

	// 5. Call the event on all collisions hit by the raycast
	for (auto& col : intersected_cols)
	{
		ECS::GetComponent(col).onRaycastIntersect.broadcast(RaycastType::RaycastTypeAABB, location); // Note: the location is not really relevent for this type of raycast
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
	raycast_comp.shapes = { std::make_shared<ShapeCube>(box, hit ? Color::red : Color::green) };

	return hit;
}

bool PhysicsManager::AABBSweepRaycast(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, float drawDebugTime, bool createOnScene)
{
	if (enableInfoLog) Locator::getLog().LogMessage_Category("Physics: Create an AABB sweep raycast.", LogCategory::Info);

	// 1. Initialize values
	outHitInfos = RaycastHitInfos();
	bool hit = false;

	// 2. Create pointers from these values so they can be passed in the lambda in read/write
	RaycastHitInfos* infos_ptr = &outHitInfos;
	bool* hit_ptr = &hit;

	// 3. Initialize the ray and the box for the computation
	Ray ray;
	ray.setupWithStartEnd(start, end);
	Box box = aabbBox;
	box.setCenterPoint(start);

	// 4. Test all collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([this, ray, box, testChannels, infos_ptr, hit_ptr](const BoxCollisionComponent& box_collision_component)
	{
		const bool col_hit = this->resolveAABBSweepRaycast(box_collision_component, ray, box, testChannels, *infos_ptr, false);
		*hit_ptr = *hit_ptr || col_hit;
	});

	// 5. Call the event on the collision hit by the raycast
	if (ECS::IsComponentHandleValid(outHitInfos.hitCollision))
	{
		ECS::GetComponent(outHitInfos.hitCollision).onRaycastIntersect.broadcast(RaycastType::RaycastTypeAABBSweep, outHitInfos.hitLocation);
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
			std::make_shared<ShapeCube>(box, Color::green),
			std::make_shared<ShapeLine>(outHitInfos.hitLocation, end, Color::red),
			std::make_shared<ShapeCube>(Box{ outHitInfos.hitLocation, box.getHalfExtents() }, Color::red)
		};
	}
	else
	{
		raycast_comp.shapes = {
			std::make_shared<ShapeLine>(start, end, Color::green),
			std::make_shared<ShapeCube>(box, Color::green),
			std::make_shared<ShapeCube>(Box{ end, box.getHalfExtents() }, Color::green)
		};
	}

	return hit;
}

bool PhysicsManager::AABBSweepPhysicTest(const Vector3& start, const Vector3& end, const Box& aabbBox, const std::vector<std::string> testChannels, Entity* testedEntity, RaycastHitInfos& outHitInfos)
{
	// 1. Initialize values
	outHitInfos = RaycastHitInfos();
	bool hit = false;

	// 2. Create pointers from these values so they can be passed in the lambda in read/write
	RaycastHitInfos* infos_ptr = &outHitInfos;
	bool* hit_ptr = &hit;

	// 3. Initialize the ray and the box for the computation
	Ray ray;
	ray.setupWithStartEnd(start, end);
	Box box = aabbBox;
	box.setCenterPoint(start);

	// 4. Test all collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([this, ray, box, testChannels, infos_ptr, hit_ptr, testedEntity](const BoxCollisionComponent& box_collision_component)
	{
		if (box_collision_component.getOwner() == testedEntity) return;

		const bool col_hit = this->resolveAABBSweepRaycast(box_collision_component, ray, box, testChannels, *infos_ptr, true);
		*hit_ptr = *hit_ptr || col_hit;
	});

	return hit;
}


const Vector3& PhysicsManager::GetGravityValue()
{
	return GRAVITY;
}

void PhysicsManager::SetEnableInfoLogs(bool enable)
{
	enableInfoLog = enable;
}

void PhysicsManager::UpdatePhysics(float dt)
{
	// Reset 'debugIntersectedLastFrame' parameter on box collision components
	auto& box_collisions_manager = ECS::Manager<BoxCollisionComponent>();
	box_collisions_manager.ForEach([](BoxCollisionComponent& box_collision_component)
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


	// Prepare all rigidbodies before collisions computation
	auto& rigidbodies_manager = ECS::Manager<RigidbodyComponent>();
	rigidbodies_manager.ForEach([dt](RigidbodyComponent& rigidbody_component)
	{
		rigidbody_component.updatePhysicsPreCollision(dt);
	});

	// Compute collisions of all rigidbodies
	rigidbodies_manager.ForEach([this](RigidbodyComponent& rigidbody_component)
	{
		this->testRigidbodyCollisions(rigidbody_component);
	});

	// Finish the update of all rigidbodies after collisions computation
	rigidbodies_manager.ForEach([dt](RigidbodyComponent& rigidbody_component)
	{
		rigidbody_component.updatePhysicsPostCollision(dt);
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

bool PhysicsManager::resolveAABBRaycast(const BoxCollisionComponent& boxColComponent, const Box& box, const std::vector<std::string> testChannels, std::vector<ComponentHandle<BoxCollisionComponent>>& intersectedCols)
{
	// 1. Check if the collision component can be tested by the raycast
	if (!channelTest(boxColComponent.collisionChannel, testChannels)) return false;

	// 2. Compute the AABB raycast intersection
	const bool hit = CollisionsAABB::IntersectAABBRaycast(boxColComponent, box);

	// 3. Register the collision in the intersected list
	if (hit) intersectedCols.push_back(boxColComponent.getSelfHandle<BoxCollisionComponent>());

	return hit;
}

bool PhysicsManager::resolveAABBSweepRaycast(const BoxCollisionComponent& boxColComponent, const Ray& ray, const Box& box, const std::vector<std::string> testChannels, RaycastHitInfos& outHitInfos, bool forCollisionTest)
{
	// 1. Check if the collision component can be tested by the raycast
	if (!channelTest(boxColComponent.collisionChannel, testChannels)) return false;

	// 2. Compute the AABB sweep raycast intersection
	return CollisionsAABB::IntersectAABBSweepRaycast(boxColComponent, ray, box, outHitInfos, forCollisionTest);
}

void PhysicsManager::testRigidbodyCollisions(RigidbodyComponent& rigidbodyComponent)
{
	// Kinematic rigidbodies update their movement outside of the physic phase
	if (rigidbodyComponent.isKinematic) return;

	if (!rigidbodyComponent.isAssociatedCollisionValid()) return;

	if (rigidbodyComponent.isFirstFrame()) return;


	// 1. Compute the rigidbody movement with the Collide and Slide algorithm
	Vector3 computed_movement = Vector3::zero;
	std::vector<CollisionHit> col_responses;
	std::vector<ComponentHandle<BoxCollisionComponent>> triggers_detected;
	bool hit = CollisionTests::RigidbodyCollideAndSlideAABB(rigidbodyComponent, false, computed_movement, col_responses, triggers_detected);

	if (hit)
	{
		// 2. Set the debug intersected value and call event on rigidbody
		ECS::GetComponent(rigidbodyComponent.getAssociatedCollision()).debugIntersectedLastFrame = true;
		rigidbodyComponent.onCollisionRepulse.broadcast(ECS::GetComponent(col_responses[0].collisionComponent), col_responses[0].impactNormal);

		// 3. Set the debug intersected values and call events on collisions encountered
		for (auto& col_response : col_responses)
		{
			BoxCollisionComponent& collision_component = ECS::GetComponent(col_response.collisionComponent);

			collision_component.debugIntersectedLastFrame = true;
			collision_component.onCollisionIntersect.broadcast(rigidbodyComponent, col_response.impactNormal);
		}
	}


	// 4. Compute the rigidbody gravity movemement with the Collide and Slide algorithm
	Vector3 computed_gravity_movement = Vector3::zero;
	if (rigidbodyComponent.useGravity)
	{
		col_responses.clear();
		hit = CollisionTests::RigidbodyCollideAndSlideAABB(rigidbodyComponent, true, computed_gravity_movement, col_responses, triggers_detected);

		if (hit)
		{
			// 5. Set the debug intersected value, call event on rigidbody and check ground value on rigidbody
			ECS::GetComponent(rigidbodyComponent.getAssociatedCollision()).debugIntersectedLastFrame = true;
			const BoxCollisionComponent& main_collision = ECS::GetComponent(col_responses[0].collisionComponent);
			rigidbodyComponent.onCollisionRepulse.broadcast(main_collision, col_responses[0].impactNormal);
			rigidbodyComponent.checkOnGround(main_collision, col_responses[0].impactNormal);

			// 6. Set the debug intersected values and call events on collisions encountered
			for (auto& col_response : col_responses)
			{
				BoxCollisionComponent& collision_component = ECS::GetComponent(col_response.collisionComponent);

				collision_component.debugIntersectedLastFrame = true;
				collision_component.onCollisionIntersect.broadcast(rigidbodyComponent, col_response.impactNormal);
			}
		}
	}


	// 7. Send the computed movements to the rigidbody
	rigidbodyComponent.applyComputedMovements(computed_movement, computed_gravity_movement);

	// 8. Call events on detected triggers
	for (auto& trigger_col : triggers_detected)
	{
		BoxCollisionComponent& trigger_col_comp = ECS::GetComponent(trigger_col);

		// If the triggered collision is managed by a non-kinematic rigidbody, call the rigidbody trigger event
		if (ECS::IsComponentHandleValid(trigger_col_comp.getOwningRigidbody()))
		{
			RigidbodyComponent& trigger_rigidbody_comp = ECS::GetComponent(trigger_col_comp.getOwningRigidbody());
			if (!trigger_rigidbody_comp.isKinematic)
			{
				trigger_rigidbody_comp.onRigidbodyTriggerEnter.broadcast(rigidbodyComponent);
				continue;
			}
		}

		trigger_col_comp.onTriggerEnter.broadcast(rigidbodyComponent);
	}
}
