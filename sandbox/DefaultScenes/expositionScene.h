#pragma once
#include <Core/scene.h>


class ExpositionScene : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	ComponentHandle<class CameraComponent> cameraOne;
	ComponentHandle<class CameraComponent> cameraTwo;
	ComponentHandle<class CameraComponent> activeCamera;

	ComponentHandle<class TextComponent> camText;
	ComponentHandle<class SpriteComponent> sprite;

	ComponentHandle<class AudioSourceComponent> musicSource;

	ComponentHandle<class ModelRendererComponent> crateModel;

	Entity* gravityCrate{ nullptr };
	Entity* whiteCube{ nullptr };
	Entity* cyanCube{ nullptr };
	float cyanCubeTimer{ 0.0f };

	const float cameraSpeed{ 4.0f };
	const float cameraSensitivity{ 0.1f };

	int logCounter{ 0 };
	bool useMatrixMaterial{ false };
};