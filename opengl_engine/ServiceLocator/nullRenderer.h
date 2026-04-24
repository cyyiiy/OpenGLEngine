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

	void AddLight(LightComponent* light) override {}
	void RemoveLight(LightComponent* light) override {}

	void AddText(TextRendererComponent* text) override {}
	void RemoveText(TextRendererComponent* text) override {}

	void AddSprite(SpriteRendererComponent* sprite) override {}
	void RemoveSprite(SpriteRendererComponent* sprite) override {}

	void DrawDebugPoint(const Vector3& pointPosition, const Color& color, float duration) override {}
	void DrawDebugLine(const Vector3& pointA, const Vector3& pointB, const Color& color, float duration) override {}
	void DrawDebugCube(const Box& boxInfos, const Color& color, float duration) override {}
};