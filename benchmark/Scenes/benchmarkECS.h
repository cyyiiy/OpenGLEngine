#pragma once
#include <Core/scene.h>
#include <ECS/ecsTypes.h>

class TextComponent;


class BenchmarkECS: public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	Entity* dummyEntity{ nullptr };
	int sharedCounter;

	ComponentHandle<TextComponent> counterText;
};

