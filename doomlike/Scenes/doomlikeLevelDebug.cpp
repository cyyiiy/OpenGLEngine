#include "doomlikeLevelDebug.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <GameComponents/movingPlatformComponent.h>

#include <PrefabFactories/floorCeilingFactory.h>
#include <PrefabFactories/stairFactory.h>


void DoomlikeLevelDebug::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });


	// Prefabs
	FloorCeilingFactory::CreateFloor(this, Vector3{  0.0f, 0.0f,  0.0f }, Vector2{ 10.0f, 10.0f }, false);
	FloorCeilingFactory::CreateFloor(this, Vector3{  0.0f, 0.0f, 10.0f }, Vector2{ 10.0f, 10.0f }, false);
	FloorCeilingFactory::CreateFloor(this, Vector3{ 10.0f, 0.0f, 10.0f }, Vector2{ 10.0f, 10.0f }, false);
	StairFactory::CreateStair(this, StairFactory::StairFacingDirection::StairFacingNegativeX, Vector3{ 4.0f, 0.0f, 2.5f });

	// Entities
	Entity* crate1 = createEntity();
	Entity* crate2 = createEntity();
	Entity* crate3 = createEntity();
	Entity* crate4 = createEntity();
	Entity* crate5 = createEntity();
	Entity* lowcrate1 = createEntity();
	Entity* lowcrate2 = createEntity();
	Entity* movingPlatform1 = createEntity();
	Entity* movingPlatform2 = createEntity();
	Entity* movingPlatform3 = createEntity();
	Entity* trigger_zone = createEntity();
	Entity* dir_light = createEntity();
	Entity* point_light = createEntity();

	crate1->setPosition(Vector3{ 2.0f, 0.5f, 0.0f });
	crate2->setPosition(Vector3{ 3.0f, 1.0f, 13.0f });
	crate2->setScale(Vector3{ 0.5f, 2.0f, 2.0f });
	crate3->setPosition(Vector3{ 9.5f, 0.5f, 9.0f });
	crate4->setPosition(Vector3{ 2.5f, 3.0f, 7.5f });
	crate5->setPosition(Vector3{ 12.0f, 2.1f, 12.0f });
	crate5->setScale(Vector3{ 1.0f, 0.1f, 1.0f });
	lowcrate1->setPosition(Vector3{ 8.0f, 0.1f, 15.5f });
	lowcrate1->setScale(Vector3{ 1.0f, 0.2f, 1.0f });
	lowcrate2->setPosition(Vector3{ 9.0f, 0.3f, 15.5f });
	lowcrate2->setScale(Vector3{ 1.0f, 0.2f, 1.0f });
	movingPlatform1->setPosition(Vector3{ 6.0f, 1.9f, 2.5f });
	movingPlatform1->setScale(Vector3{ 2.0f, 0.2f, 2.0f });
	movingPlatform2->setPosition(Vector3{ -7.0f, 0.0f, -7.0f });
	movingPlatform2->setScale(Vector3{ 2.0f, 0.2f, 2.0f });
	movingPlatform3->setPosition(Vector3{ 10.0f, -3.0f, 3.0f });
	movingPlatform3->setScale(Vector3{ 2.0f, 0.2f, 2.0f });
	trigger_zone->setPosition(Vector3{ 12.0f, 1.0f, 12.0f });

	// Model components
	ECS::GetComponent(crate1->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate2->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate3->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate4->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate5->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(lowcrate1->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(lowcrate2->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(movingPlatform1->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(movingPlatform2->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(movingPlatform3->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");

	// Collision components
	ECS::GetComponent(crate1->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate2->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate3->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate4->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate5->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(lowcrate1->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(lowcrate2->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	BoxCollisionComponent& trigger_zone_col = ECS::GetComponent(trigger_zone->addComponentByClass<BoxCollisionComponent>());
	trigger_zone_col.collisionChannel = "trigger_zone";
	trigger_zone_col.isTrigger = true;

	// Moving platform components
	ECS::GetComponent(movingPlatform1->addComponentByClass<MovingPlatformComponent>()).setupMovingPlatform(Vector3{ 6.0f, 1.9f, 2.5f }, Vector3{ 9.0f, 3.0f, 11.5f }, 3.0f, 2.0f);
	ECS::GetComponent(movingPlatform2->addComponentByClass<MovingPlatformComponent>()).setupMovingPlatform(Vector3{ -7.0f, 0.0f, -7.0f }, Vector3{ -7.0f, 0.0f, 7.0f }, 3.0f);
	ECS::GetComponent(movingPlatform3->addComponentByClass<MovingPlatformComponent>()).setupMovingPlatform(Vector3{ 10.0f, -3.0f, 3.0f }, Vector3{ 10.0f, 5.0f, 3.0f }, 3.0f);

	// Light components
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(dir_light->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.lightColor = Color::white;
	dir_light_comp.direction = Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f });
	dir_light_comp.ambientStrength = 0.1f;
	dir_light_comp.diffuseStrength = 0.7f;

	PointLightComponent& point_light_comp = ECS::GetComponent(point_light->addComponentByClass<PointLightComponent>());
	point_light_comp.lightColor = Color::red;
	point_light_comp.offset = Vector3{ 8.0f, 1.0f, 9.0f };


	// Player spawn point
	spawnPoint = createEntity();
	spawnPoint->setPosition(Vector3{ 8.0f, 0.0f, 9.0f });
	spawnPoint->setRotation(Quaternion::fromEuler(Maths::toRadians(135.0f), 0.0f, 0.0f));
}

void DoomlikeLevelDebug::unloadScene()
{
}

void DoomlikeLevelDebug::updateScene(float dt)
{
}