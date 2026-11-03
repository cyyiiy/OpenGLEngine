#pragma once
#include <Core/scene.h>

class DemoScene : public Scene
{
protected:
	void loadScene() override;
	void unloadScene() override;
	void updateScene(float dt) override;

private:
	Entity* cameraEntity;
	ComponentHandle<class CameraComponent> cameraComp;
};