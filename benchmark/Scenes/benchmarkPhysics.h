#pragma once
#include <Core/scene.h>


class BenchmarkPhysics : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:

};

