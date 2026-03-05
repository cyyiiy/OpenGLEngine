#pragma once
#include <Core/scene.h>


class BenchmarkRendering3D : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	Entity* camera{ nullptr };
	float cameraTimer{ 0.0f };
};

