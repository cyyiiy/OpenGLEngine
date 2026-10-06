#pragma once
#include <Core/scene.h>


class BenchmarkMaterials : public Scene
{
public:
	std::string getSceneName() override { return "BenchmarkMaterialsScene"; }

protected:
	void loadSceneAssets() override;
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	Entity* camera{ nullptr };
	float cameraTimer{ 0.0f };
};

