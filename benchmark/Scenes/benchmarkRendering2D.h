#pragma once
#include <Core/scene.h>


class BenchmarkRendering2D : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	std::vector<std::shared_ptr<class TextRendererComponent>> spinning_texts;
};

