#pragma once
#include <Core/game.h>

class BenchmarkGame : public Game
{

protected:
	void loadGameAssets() override;
	void loadGame() override;
	void unloadGame() override;
	void updateGame(float dt) override;
};

