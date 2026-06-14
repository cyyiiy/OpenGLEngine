#pragma once
#include <ServiceLocator/renderer.h>
#include <Rendering/cameraComponent.h>
#include <Utils/Color.h>
#include <Maths/Vector3.h>
#include <Maths/Geometry/box.h>
#include <stdexcept>

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
};