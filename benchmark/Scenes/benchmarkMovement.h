#pragma once
#include <Core/scene.h>
#include <vector>


class BenchmarkMovement : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
};

