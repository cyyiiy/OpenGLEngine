#pragma once
#include <Core/game.h>
#include <Scenes/testFpsScene.h>
#include <Scenes/doomlikeLevelDebug.h>
#include <Scenes/doomlikeLevelStart.h>
#include <Scenes/doomlikeLevelAdvanced.h>

class PlayerComponent;


class DoomlikeGame : public Game
{
public:
	std::weak_ptr<PlayerComponent> getPlayer() { return player; }

	void restartLevel();
	void changeLevel(int levelIndex);

protected:
	void loadGameAssets() override;
	void loadGame() override;
	void unloadGame() override;
	void updateGame(float dt) override;

private:
	void loadLevel(int index);

	//  scenes
	TestFpsScene testScene;
	DoomlikeLevelDebug levelDebugScene;
	DoomlikeLevelStart levelStartScene;
	DoomlikeLevelAdvanced levelAdvancedScene;
	int currentLevel = 0;
	bool mustRestartLevel{ false };

	//  player
	std::shared_ptr<PlayerComponent> player;
};

