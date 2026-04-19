#pragma once
#include <Core/game.h>
//#include <DefaultScenes/expositionScene.h>
#include <DefaultScenes/wipScene.h>


class ExpositionGame : public Game
{
public:
	ExpositionGame();

	void updateGame(float dt) override;

protected:
	void loadGameAssets() override;
	void loadGame() override;
	void unloadGame() override;

private:
	//ExpositionScene expositionScene;
	WipScene wipScene;

	Entity* test_game_entity{ nullptr };
};