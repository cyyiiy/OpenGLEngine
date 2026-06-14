#pragma once
#include <Core/scene.h>
#include <Events/observer.h>
#include <GameLogic/playerSpawnPoint.h>
#include <GameLogic/enemyCount.h>

class RigidbodyComponent;


class DoomlikeLevelStart : public Scene, public Observer, public PlayerSpawnPoint
{
public:
	void updateScene(float dt) override;


protected:
	void loadScene() override;
	void unloadScene() override;


private:
	// Level utilities
	EnemyCount enemyCount;

	Entity* endLevelWall;
	Entity* endLevelZone;

	void onEnemiesDead();
	void onEnterEndLevelZone(const RigidbodyComponent& body);
};