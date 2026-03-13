#pragma once
#include <Core/scene.h>


class BenchmarkECS: public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	Entity* dummyEntity{ nullptr };
	int sharedCounter;

	std::shared_ptr<class TextRendererComponent> counterText;
};

