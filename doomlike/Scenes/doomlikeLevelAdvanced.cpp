#include "doomlikeLevelAdvanced.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>
#include <GameComponents/lampComponent.h>
#include <GameComponents/movingPlatformComponent.h>
//#include <GameComponents/enemyComponent.h>

#include <PrefabFactories/wallFactory.h>
#include <PrefabFactories/floorCeilingFactory.h>
#include <PrefabFactories/lampFactory.h>

using WallFactory::WallFacingDirection;


void DoomlikeLevelAdvanced::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });


	// Prefabs - static
	FloorCeilingFactory::CreateFloor(this, Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 20.0f, 20.0f }, false);
	FloorCeilingFactory::CreateFloor(this, Vector3{ 0.0f, 7.0f, 0.0f }, Vector2{  2.5f,  2.5f }, true);
	FloorCeilingFactory::CreateCeiling(this, Vector3{ 0.0f, 10.0f, 0.0f }, Vector2{ 20.0f, 20.0f });

	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeZ, Vector3{   0.0f, 3.5f, -1.25f }, Vector2{ 2.5f, 7.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{  1.25f, 3.5f,   0.0f }, Vector2{ 2.5f, 7.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeX, Vector3{ -1.25f, 3.5f,   0.0f }, Vector2{ 2.5f, 7.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{   0.0f, 3.5f,  1.25f }, Vector2{ 2.5f, 7.0f }, true);

	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeZ, Vector3{   0.0f, 5.0f,  10.0f }, Vector2{ 20.0f, 10.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{ -10.0f, 5.0f,   0.0f }, Vector2{ 20.0f, 10.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeX, Vector3{  10.0f, 5.0f,   0.0f }, Vector2{ 20.0f, 10.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{   0.0f, 5.0f, -10.0f }, Vector2{ 20.0f, 10.0f }, false);

	// Prefabs - dynamic
	ceilLamp1 = LampFactory::CreateLamp(this, Vector3{ -8.0f, 10.0f, -8.0f }, 1.0f, true, true);
	ceilLamp2 = LampFactory::CreateLamp(this, Vector3{  8.0f, 10.0f, -8.0f }, 1.0f, true, true);
	ceilLamp3 = LampFactory::CreateLamp(this, Vector3{ -8.0f, 10.0f,  8.0f }, 1.0f, true, true);
	ceilLamp4 = LampFactory::CreateLamp(this, Vector3{  8.0f, 10.0f,  8.0f }, 1.0f, true, true);
	ceilLamp5 = LampFactory::CreateLamp(this, Vector3{ -8.0f, 10.0f,  0.0f }, 1.0f, true, true);
	ceilLamp6 = LampFactory::CreateLamp(this, Vector3{  8.0f, 10.0f,  0.0f }, 1.0f, true, true);
	ceilLamp7 = LampFactory::CreateLamp(this, Vector3{  0.0f, 10.0f, -8.0f }, 1.0f, true, true);
	ceilLamp8 = LampFactory::CreateLamp(this, Vector3{  0.0f, 10.0f,  8.0f }, 1.0f, true, true);
	floorLamp = LampFactory::CreateLamp(this, Vector3{  6.0f,  0.0f,  0.0f }, 2.0f, false, false);

	// Directional light
	Entity* light = createEntity();
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(light->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.lightColor = Color{ 255, 238, 209, 255 };
	dir_light_comp.direction = Vector3::unitY;
	dir_light_comp.ambientStrength = 0.15f;
	dir_light_comp.diffuseStrength = 0.0f;


	// Elevator
	elevator = createEntity();
	elevator->setPosition(Vector3{ 2.5f, 0.1f, 0.0f });
	elevator->setScale(Vector3{ 2.0f, 0.2f, 2.0f });
	ECS::GetComponent(elevator->addComponentByClass<ModelRendererComponent>()).model = &AssetManager::GetModel("crate");
	MovingPlatformComponent& elevator_comp = ECS::GetComponent(elevator->addComponentByClass<MovingPlatformComponent>());
	elevator_comp.setupMovingPlatform(Vector3{ 2.5f, 0.1f, 0.0f }, Vector3{ 2.5f, 6.9f, 0.0f }, 2.5f, 2.0f);
	elevator_comp.pauseMovement();


	// Trigger zones
	elevatorUpZone = createEntity();
	elevatorUpZone->setPosition(Vector3{ 2.5f, 1.0f, 0.0f });
	elevatorUpZone->setScale(Vector3{ 0.3f, 0.3f, 0.3f });
	BoxCollisionComponent& elevator_trigger_comp = ECS::GetComponent(elevatorUpZone->addComponentByClass<BoxCollisionComponent>());
	elevator_trigger_comp.collisionBox = Box::one;
	elevator_trigger_comp.collisionChannel = "trigger_zone";
	elevator_trigger_comp.isTrigger = true;
	elevator_trigger_comp.onTriggerEnter.subscribe(this, &DoomlikeLevelAdvanced::onEnterElevatorUpZone);

	enemySpawnZone = createEntity();
	enemySpawnZone->setPosition(Vector3{ 0.0f, 7.5f, 0.0f });
	enemySpawnZone->setScale(Vector3{ 0.2f, 0.2f, 0.2f });
	BoxCollisionComponent& enemy_trigger_comp = ECS::GetComponent(enemySpawnZone->addComponentByClass<BoxCollisionComponent>());
	enemy_trigger_comp.collisionBox = Box::one;
	enemy_trigger_comp.collisionChannel = "trigger_zone";
	enemy_trigger_comp.isTrigger = true;
	enemy_trigger_comp.onTriggerEnter.subscribe(this, &DoomlikeLevelAdvanced::onEnterEnemySpawnZone);


	// Player spawn point
	spawnPoint = createEntity();
	spawnPoint->setPosition(Vector3{ 9.0f, 0.0f, 0.0f });
	spawnPoint->setRotation(Quaternion::fromEuler(Maths::toRadians(180.0f), 0.0f, 0.0f));
}

void DoomlikeLevelAdvanced::unloadScene()
{
}

void DoomlikeLevelAdvanced::updateScene(float dt)
{
}

void DoomlikeLevelAdvanced::onEnterElevatorUpZone(const RigidbodyComponent& body)
{
	if (!body.getOwner()->hasGameplayTag("Player")) return;

	ECS::GetComponent(elevatorUpZone->getComponentOfClass<BoxCollisionComponent>()).onTriggerEnter.unsubscribe(this);
	ECS::GetComponent(elevator->getComponentOfClass<MovingPlatformComponent>()).resumeMovement();
}

void DoomlikeLevelAdvanced::onEnterEnemySpawnZone(const RigidbodyComponent& body)
{
	if (!body.getOwner()->hasGameplayTag("Player")) return;

	// Change lights
	ECS::GetComponent(ceilLamp1->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp2->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp3->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp4->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp5->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp6->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp7->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(ceilLamp8->getComponentOfClass<LampComponent>()).changeStatus(true);
	ECS::GetComponent(floorLamp->getComponentOfClass<LampComponent>()).changeStatus(false);

	// Spawn enemies
	/*
	Entity* enemy_1 = createEntity();
	Entity* enemy_2 = createEntity();
	Entity* enemy_3 = createEntity();
	Entity* enemy_4 = createEntity();
	enemy_1->setPosition(Vector3{ -5.0f, 7.5f, -5.0f });
	enemy_2->setPosition(Vector3{ -5.0f, 7.5f,  5.0f });
	enemy_3->setPosition(Vector3{  5.0f, 7.5f, -5.0f });
	enemy_4->setPosition(Vector3{  5.0f, 7.5f,  5.0f });
	enemy_1->addComponentByClass<EnemyComponent>();
	enemy_2->addComponentByClass<EnemyComponent>();
	enemy_3->addComponentByClass<EnemyComponent>();
	enemy_4->addComponentByClass<EnemyComponent>();
	*/

	ECS::GetComponent(enemySpawnZone->getComponentOfClass<BoxCollisionComponent>()).onTriggerEnter.unsubscribe(this);
}
