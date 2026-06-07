#pragma once
#include <Core/scene.h>
#include <ECS/ecsTypes.h>

class TextComponent;


class BenchmarkRendering2D : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	std::vector<ComponentHandle<TextComponent>> spinning_texts;
};

