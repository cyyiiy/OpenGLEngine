#pragma once
#include <Events/event.h>
#include <Events/observer.h>
#include <vector>

class Entity;


class EnemyCount : public Observer
{
public:
	void addEnemy(Entity* enemyToAdd);
	void addEnemies(std::vector<Entity*> enemiesToAdd);

	void clearEnemyCount(bool clearEvent);

	Event<> onAllEnemiesDead;

private:
	std::vector<Entity*> enemies;

	void onEnemyDie(Entity* enemyDead);
};