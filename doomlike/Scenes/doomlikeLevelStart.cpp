#include "doomlikeLevelStart.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <GameplayStatics/gameplayStatics.h>
#include <doomlikeGame.h>

#include <Rendering/Lights/directionalLightComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>
#include <GameComponents/enemyComponent.h>

#include <PrefabFactories/wallFactory.h>
#include <PrefabFactories/floorCeilingFactory.h>
#include <PrefabFactories/stairFactory.h>
#include <PrefabFactories/lampFactory.h>

using WallFactory::WallFacingDirection;


void DoomlikeLevelStart::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });


	// Prefabs
	FloorCeilingFactory::CreateFloor(this, Vector3{ 0.0f, 0.0f,  2.5f }, Vector2{  5.0f, 10.0f }, true);
	FloorCeilingFactory::CreateFloor(this, Vector3{ 0.0f, 0.0f, 15.0f }, Vector2{ 15.0f, 15.0f }, true);
	FloorCeilingFactory::CreateCeiling(this, Vector3{ 0.0f, 3.0f,  2.5f }, Vector2{  5.0f, 10.0f });
	FloorCeilingFactory::CreateCeiling(this, Vector3{ 0.0f, 5.0f, 15.0f }, Vector2{ 15.0f, 15.0f });

	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{  0.0f, 1.5f, -2.5f }, Vector2{  5.0f, 3.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{ -2.5f, 1.5f,  2.5f }, Vector2{ 10.0f, 3.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeX, Vector3{  2.5f, 1.5f,  2.5f }, Vector2{ 10.0f, 3.0f }, false);

	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{ -5.0f, 2.5f,   7.5f }, Vector2{  5.0f, 5.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{  0.0f, 4.0f,   7.5f }, Vector2{  5.0f, 2.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{  5.0f, 2.5f,   7.5f }, Vector2{  5.0f, 5.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeX, Vector3{  7.5f, 2.5f,  15.0f }, Vector2{ 15.0f, 5.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeZ, Vector3{ -5.0f, 2.5f,  22.5f }, Vector2{ 25.0f, 5.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{ -7.5f, 2.5f, 12.25f }, Vector2{  9.5f, 5.0f }, false);

	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeZ, Vector3{  -2.5f, 2.5f, 11.75f }, Vector2{ 1.5f, 5.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{ -1.75f, 2.5f,  12.5f }, Vector2{ 1.5f, 5.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeX, Vector3{ -3.25f, 2.5f,  12.5f }, Vector2{ 1.5f, 5.0f }, true);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{  -2.5f, 2.5f, 13.25f }, Vector2{ 1.5f, 5.0f }, true);

	StairFactory::CreateStair(this, StairFactory::StairFacingDirection::StairFacingNegativeZ, Vector3{ 3.8f, 0.0f, 16.0f });
	FloorCeilingFactory::CreateFloor(this, Vector3{ -5.0f, 2.0f, 19.75f }, Vector2{ 25.0f, 5.5f }, true);
	FloorCeilingFactory::CreateCeiling(this, Vector3{ -12.5f, 5.0f, 19.75f }, Vector2{ 10.0f, 5.5f });
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingNegativeZ, Vector3{   0.0f, 1.0f, 17.0f }, Vector2{ 15.0f, 2.0f }, false);
	WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveZ, Vector3{ -12.5f, 3.5f, 17.0f }, Vector2{ 10.0f, 3.0f }, false);

	endLevelWall = WallFactory::CreateWall(this, WallFacingDirection::WallFacingPositiveX, Vector3{ -7.5f, 3.5f, 19.75f }, Vector2{ 5.5f, 3.0f }, false);

	LampFactory::CreateLamp(this, Vector3{ -3.2f, 0.0f,  8.3f }, 2.3f, false);
	LampFactory::CreateLamp(this, Vector3{  3.2f, 0.0f,  8.3f }, 2.3f, false);
	LampFactory::CreateLamp(this, Vector3{  6.5f, 2.0f, 21.5f }, 2.3f, false);
	LampFactory::CreateLamp(this, Vector3{ -3.0f, 5.0f, 16.5f }, 1.7f, true);

	// Directional light
	Entity* light = createEntity();
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(light->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.lightColor = Color{ 255, 238, 209, 255 };
	dir_light_comp.direction = Vector3::unitY;
	dir_light_comp.ambientStrength = 0.35f;
	dir_light_comp.diffuseStrength = 0.0f;


	// Enemies
	Entity* enemy_1 = createEntity();
	Entity* enemy_2 = createEntity();
	enemy_1->setPosition(Vector3{ 3.5f, 1.2f,  11.5f });
	enemy_2->setPosition(Vector3{ -3.5f, 3.2f, 20.0f });
	enemy_1->addComponentByClass<EnemyComponent>();
	enemy_2->addComponentByClass<EnemyComponent>();

	enemyCount.addEnemies({ enemy_1, enemy_2 });
	enemyCount.onAllEnemiesDead.subscribe(this, &DoomlikeLevelStart::onEnemiesDead);


	// Trigger zone
	endLevelZone = createEntity();
	endLevelZone->setPosition(Vector3{ -15.0f, 3.5f, 19.75f });
	endLevelZone->setScale(Vector3{ 4.0f, 2.5f, 4.8f });
	BoxCollisionComponent& trigger_comp = ECS::GetComponent(endLevelZone->addComponentByClass<BoxCollisionComponent>());
	trigger_comp.collisionBox = Box::one;
	trigger_comp.collisionChannel = "trigger_zone";
	trigger_comp.isTrigger = true;
	trigger_comp.onTriggerEnter.subscribe(this, &DoomlikeLevelStart::onEnterEndLevelZone);


	// Player spawn point
	spawnPoint = createEntity();
	spawnPoint->setPosition(Vector3::zero);
	spawnPoint->setRotation(Quaternion::fromEuler(Maths::toRadians(-90.0f), 0.0f, 0.0f));
}

void DoomlikeLevelStart::unloadScene()
{
	enemyCount.clearEnemyCount(true);
}

void DoomlikeLevelStart::updateScene(float dt)
{
}

void DoomlikeLevelStart::onEnemiesDead()
{
	Locator::getLog().LogMessageToScreen("Doomlike Intro Level: All enemies of the level are dead!", Color::white, 5.0f);
	endLevelWall->setPosition(Vector3{ -17.5f, 3.5f, 19.75f });
}

void DoomlikeLevelStart::onEnterEndLevelZone(const RigidbodyComponent& body)
{
	if (!body.getOwner()->hasGameplayTag("Player")) return;

	Locator::getLog().LogMessageToScreen("Doomlike Intro Level: Player exit intro level.", Color::white, 5.0f);
	static_cast<DoomlikeGame*>(GameplayStatics::GetGame())->changeLevel(3);

	ECS::GetComponent(endLevelZone->getComponentOfClass<BoxCollisionComponent>()).onTriggerEnter.unsubscribe(this);
}