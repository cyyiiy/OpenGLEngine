#pragma once
#include <ServiceLocator/renderer.h>

#include <Utils/color.h>
#include <Maths/matrix4.h>
#include <Maths/vector3.h>
#include <Maths/vector2Int.h>
#include <Maths/vector4.h>
#include <Maths/Geometry/box.h>
#include <ECS/ecsTypes.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/material.h>

#include <vector>
#include <unordered_map>
#include <memory>


enum LightType : uint8_t
{
	EDirectionalLight,
	EPointLight,
	ESpotLight
};

// Hard limit coded in the shader of how many lights of each type can be active at once
const std::unordered_map<LightType, int> LIGHTS_LIMITS
{
	{ EDirectionalLight, 1 },
	{ EPointLight, 64 },
	{ ESpotLight, 32 }
};


const int TEXT_CHARS_LIMIT{ 200 };


/**
* The renderer service provider class.
*/
class RendererOpenGL : public Renderer
{
public:
	void SetCamera(ComponentHandle<CameraComponent> camera) override;
	bool IsActiveCamera(ComponentHandle<CameraComponent> camera) override;
	void RemoveActiveCamera() override;
	const CameraComponent& GetCamera() const override;

	void SetClearColor(Color clearColor_) override;
	const Color GetClearColor() const override;

	void AddMaterial(Material* material) override;
	void RemoveMaterial(Material* material) override;


	void InitializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_) override;
	void Draw() override;

	void SetDebugCamera(ComponentHandle<CameraComponent> debugCamera_) override;
	void SetDebugCamActivated(bool debugCamActivated_) override;
	void SetDebugViewMode(bool debugViewMode_) override;
	void SetWindowSize(Vector2Int windowSize_) override;


private:
	CameraComponent& selectCurrentCam();
	bool isCurrentCamValid();

	void drawModelComponent(const class ModelRendererComponent& modelComponent, Material& materialInUsage);

	void useDirectionalLight(const class DirectionalLightComponent& dirLightComponent, Shader& shaderInUsage);
	void usePointLight(const class PointLightComponent& pointLightComponent, Shader& shaderInUsage);
	void useSpotLight(const class SpotLightComponent& spotLightComponent, Shader& shaderInUsage);

	void drawBillboardComponent(const class BillboardRendererComponent& billboardComponent, Shader& shaderInUsage);

	void drawBoxCollision(const class BoxCollisionComponent& boxColComponent, Shader& shaderInUsage);

	void drawTextComponent(const class TextComponent& textComponent, Shader& shaderInUsage);
	void drawSpriteComponent(const class SpriteComponent& spriteComponent, Shader& shaderInUsage);


	std::unordered_map<Shader*, std::vector<Material*>> materials;
	std::unordered_map<LightType, int> lights_count;

	Color clearColor{ Color::black };

	ComponentHandle<CameraComponent> activeCamera;
	ComponentHandle<CameraComponent> defaultCamera;
	ComponentHandle<CameraComponent> debugCamera;

	Vector2Int windowSize{ Vector2Int::zero };

	bool debugCamActivated{ false };
	bool debugViewMode{ false };
};