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
//#include <Rendering/Text/textRendererComponent.h>
//#include <Rendering/Hud/spriteRendererComponent.h>

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


//const int TEXT_CHARS_LIMIT{ 200 };


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

	//void AddText(TextRendererComponent* text) override;
	//void RemoveText(TextRendererComponent* text) override;

	//void AddSprite(SpriteRendererComponent* sprite) override;
	//void RemoveSprite(SpriteRendererComponent* sprite) override;



private:
	CameraComponent& selectCurrentCam();
	bool isCurrentCamValid();

	void drawModelComponent(const class ModelRendererComponent& modelComponent, Material& materialInUsage);

	void useDirectionalLight(const class DirectionalLightComponent& dirLightComponent, Shader& shaderInUsage);
	void usePointLight(const class PointLightComponent& pointLightComponent, Shader& shaderInUsage);
	void useSpotLight(const class SpotLightComponent& spotLightComponent, Shader& shaderInUsage);


	std::unordered_map<Shader*, std::vector<Material*>> materials;
	std::unordered_map<LightType, int> lights_count;
	//std::vector<TextRendererComponent*> texts;
	//std::vector<SpriteRendererComponent*> sprites;

	Color clearColor{ Color::black };

	ComponentHandle<CameraComponent> activeCamera;
	ComponentHandle<CameraComponent> defaultCamera;
	ComponentHandle<CameraComponent> debugCamera;

	Vector2Int windowSize;

	bool debugCamActivated;

	




//  exclusive to engine which is the only class to access the full renderer
public:
	void initializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_);

	void setDebugCamera(ComponentHandle<CameraComponent> debugCamera_);
	void setDebugCamActivated(bool debugCamActivated_);

	void draw();

	void setWindowSize(Vector2Int windowSize_);

	bool physicsDebugMode{ false };
};

