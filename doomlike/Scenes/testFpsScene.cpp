#include "testFpsScene.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <GameComponents/targetComponent.h>

#include <PrefabFactories/floorCeilingFactory.h>


void TestFpsScene::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });


	// Prefabs
	FloorCeilingFactory::CreateFloor(this, Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 10.0f, 10.0f }, false);

	// Entities
	Entity* crate1 = createEntity();
	Entity* crate2 = createEntity();
	Entity* crate3 = createEntity();
	Entity* target = createEntity();
	Entity* taxi = createEntity();
	Entity* light = createEntity();

	crate1->setPosition(Vector3{ 2.0f, 0.5f, 0.0f });
	crate2->setPosition(Vector3{ -1.0f, 0.5f, 3.0f });
	crate3->setPosition(Vector3{ -3.5f, 0.5f, -1.0f });
	target->setPosition(Vector3{ 0.0f, 1.5f, -5.0f });
	target->setScale(Vector3{ 1.0f, 1.0f, 0.1f });
	taxi->setPosition(Vector3{ -7.0f, 1.0f, 0.0f });
	taxi->setScale(0.01f);

	// Model components
	ECS::GetComponent(crate1->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate2->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(crate3->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(target->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	ECS::GetComponent(taxi->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("taxi");

	// Collision components
	ECS::GetComponent(crate1->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate2->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(crate3->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";
	ECS::GetComponent(target->addComponentByClass<BoxCollisionComponent>()).collisionChannel = "solid";

	// Custom components
	target->addComponentByClass<TargetComponent>();

	// Directional light
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(light->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.lightColor = Color::white;
	dir_light_comp.direction = Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f });
	dir_light_comp.ambientStrength = 0.1f;
	dir_light_comp.diffuseStrength = 0.7f;

	// Raycast tests
	Physics& physics = Locator::getPhysics();
	RaycastHitInfos out_raycast;
	physics.LineRaycast(Vector3{ -1.0f, 3.5f, 3.0f }, Vector3{ -1.0f, -1.5f, 3.0f }, { }, out_raycast);
	physics.LineRaycast(Vector3{ -4.5f, 0.5f, -3.0f }, Vector3{ 0.0f, 0.5f, 5.0f }, { }, out_raycast, -1.0f);
	physics.AABBRaycast(Vector3{ 4.2f, 0.3f, -2.0f }, Box::one);


	// Player spawn point
	spawnPoint = createEntity();
	spawnPoint->setPosition(Vector3::zero);
}

void TestFpsScene::unloadScene()
{
}