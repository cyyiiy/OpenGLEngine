#pragma once
#include <Core/scene.h>
#include <GameLogic/playerSpawnPoint.h>


class TestFpsScene : public Scene, public PlayerSpawnPoint
{
public:
	std::string getSceneName() override { return "TestFpsScene"; }

protected:
	void loadSceneAssets() override;
	void loadScene() override;
	void unloadScene() override;
};