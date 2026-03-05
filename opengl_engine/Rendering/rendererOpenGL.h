#pragma once
#include <ServiceLocator/renderer.h>

#include <Utils/color.h>
#include <Maths/matrix4.h>
#include <Maths/vector3.h>
#include <Maths/vector2Int.h>
#include <Maths/vector4.h>
#include <Maths/Geometry/box.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/material.h>
#include <Rendering/Lights/lightComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Text/textRendererComponent.h>
#include <Rendering/Hud/spriteRendererComponent.h>
#include <Rendering/Debug/debugRenderBase.h>

#include <vector>
#include <unordered_map>
#include <memory>


//  would be cool if I find a better way to do this but it works for now
const std::unordered_map<LightType, int> LIGHTS_LIMITS
{
	{EDirectionalLight, 1},
	{EPointLight, 64},
	{ESpotLight, 32}
};

const int TEXT_CHARS_LIMIT{ 200 };


/**
* The renderer service provider class.
*/
class RendererOpenGL : public Renderer
{
public:
	void SetCamera(std::weak_ptr<CameraComponent> camera) override;
	const std::shared_ptr<CameraComponent> GetCamera() const override;

	void SetClearColor(Color clearColor_) override;
	const Color GetClearColor() const override;

	void AddMaterial(Material* material) override;
	void RemoveMaterial(Material* material) override;

	void AddLight(LightComponent* light) override;
	void RemoveLight(LightComponent* light) override;

	void AddModelRenderer(ModelRendererComponent* modelRenderer) override;
	void RemoveModelRenderer(ModelRendererComponent* modelRenderer) override;

	void AddText(TextRendererComponent* text) override;
	void RemoveText(TextRendererComponent* text) override;

	void AddSprite(SpriteRendererComponent* sprite) override;
	void RemoveSprite(SpriteRendererComponent* sprite) override;

	void DrawDebugPoint(const Vector3& pointPosition, const Color& color, float duration) override;
	void DrawDebugLine(const Vector3& pointA, const Vector3& pointB, const Color& color, float duration) override;
	void DrawDebugCube(const Box& boxInfos, const Color& color, float duration) override;



private:
	CameraComponent& selectCurrentCam();
	bool isCurrentCamValid();

	std::unordered_map<LightType, std::vector<LightComponent*>> lights;
	std::unordered_map<Shader*, std::vector<Material*>> materials;
	std::vector<ModelRendererComponent*> modelRenderers;
	std::vector<TextRendererComponent*> texts;
	std::vector<SpriteRendererComponent*> sprites;
	std::vector<DebugRenderBase*> debugDraws;

	Color clearColor{ Color::black };

	std::shared_ptr<CameraComponent> activeCamera;
	std::shared_ptr<CameraComponent> defaultCamera;
	std::shared_ptr<CameraComponent> debugCamera;

	Vector2Int windowSize;

	bool debugCamActivated;

	




//  exclusive to engine which is the only class to access the full renderer
public:
	void initializeRenderer(Color clearColor_, Vector2Int windowSize_, std::weak_ptr<CameraComponent> defaultCamera_);

	void setDebugCamera(std::weak_ptr<CameraComponent> debugCamera_);
	void setDebugCamActivated(bool debugCamActivated_);

	void draw();
	void updateDebugDraws(float dt);

	void setWindowSize(Vector2Int windowSize_);

	bool physicsDebugMode{ false };
};

