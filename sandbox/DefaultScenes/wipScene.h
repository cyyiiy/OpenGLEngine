#pragma once
#include <Core/scene.h>
#include <Rendering/cameraComponent.h>

class WipScene : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	ComponentHandle<CameraComponent> cameraOne;
	ComponentHandle<CameraComponent> cameraTwo;
	ComponentHandle<CameraComponent> activeCamera;
	
	Entity* cyanCube{ nullptr };
	float cyanCubeTimer{ 0.0f };

	const float cameraSpeed{ 4.0f };
	const float cameraSensitivity{ 0.1f };
};

