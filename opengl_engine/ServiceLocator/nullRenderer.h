#pragma once
#include <ServiceLocator/renderer.h>
#include <Utils/Color.h>
#include <Maths/Vector3.h>
#include <Maths/Vector2Int.h>
#include <Maths/Geometry/box.h>
#include <stdexcept>

class CameraComponent;


/**
* The renderer null service provider class.
*/
class NullRenderer : public Renderer
{
public:
	void SetCamera(ComponentHandle<CameraComponent> camera) override {}
	bool IsActiveCamera(ComponentHandle<CameraComponent> camera) override { return false; }
	void RemoveActiveCamera() override {}
	const CameraComponent& GetCamera() const override { throw std::runtime_error("The renderer doesn't have an active camera."); }

	void SetClearColor(Color clearColor) override {}
	const Color GetClearColor() const override { return Color::black; }

	void AddMaterial(Material* material) override {}
	void RemoveMaterial(Material* material) override {}


	void InitializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_) override {}
	void Draw() override {}

	void SetDebugCamera(ComponentHandle<CameraComponent> debugCamera_) override {}
	void SetDebugCamActivated(bool debugCamActivated_) override {}
	void SetDebugViewMode(bool debugViewMode_) override {}
	void SetWindowSize(Vector2Int windowSize_) override {}
};