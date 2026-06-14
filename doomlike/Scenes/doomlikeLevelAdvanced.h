#pragma once
#include <Core/scene.h>
#include <Events/observer.h>
#include <GameLogic/playerSpawnPoint.h>

class RigidbodyComponent;


class DoomlikeLevelAdvanced : public Scene, public Observer, public PlayerSpawnPoint
{
public:
	void updateScene(float dt) override;


protected:
	void loadScene() override;
	void unloadScene() override;


private:
	// Dynamic lights
	Entity* ceilLamp1;
	Entity* ceilLamp2;
	Entity* ceilLamp3;
	Entity* ceilLamp4;
	Entity* ceilLamp5;
	Entity* ceilLamp6;
	Entity* ceilLamp7;
	Entity* ceilLamp8;
	Entity* floorLamp;

	// Level utilities
	Entity* elevatorUpZone;
	Entity* enemySpawnZone;

	Entity* elevator;

	void onEnterElevatorUpZone(const RigidbodyComponent& body);
	void onEnterEnemySpawnZone(const RigidbodyComponent& body);
};