#pragma once
#include <Core/scene.h>
#include <Rendering/cameraComponent.h>
#include <Rendering/Text/textComponent.h>
#include <Rendering/Hud/spriteComponent.h>
#include <Audio/audioSourceComponent.h>


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

	ComponentHandle<TextComponent> camText;
	ComponentHandle<SpriteComponent> sprite;

	ComponentHandle<AudioSourceComponent> musicSource;
	
	Entity* whiteCube{ nullptr };
	Entity* cyanCube{ nullptr };
	float cyanCubeTimer{ 0.0f };

	const float cameraSpeed{ 4.0f };
	const float cameraSensitivity{ 0.1f };

	int logCounter{ 0 };
};

