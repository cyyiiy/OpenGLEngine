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
};

