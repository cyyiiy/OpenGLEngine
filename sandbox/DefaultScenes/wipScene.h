#pragma once
#include <Core/scene.h>

class WipScene : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;
};

