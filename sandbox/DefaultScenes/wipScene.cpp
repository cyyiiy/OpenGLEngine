#include "wipScene.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Inputs/Input.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/Lights/spotLightComponent.h>


void WipScene::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Models

	Entity* floor = createEntity();
	floor->setScale(10.0f);
	ModelRendererComponent& floor_model = ECS::GetComponent(floor->addComponentByClass<ModelRendererComponent>());
	floor_model.model = &AssetManager::GetModel("floor_wood");

	Entity* white_cube = createEntity();
	white_cube->setPosition(Vector3{ -2.0f, 0.4f, 1.5f });
	white_cube->setScale(0.2f);
	ModelRendererComponent& white_cube_model = ECS::GetComponent(white_cube->addComponentByClass<ModelRendererComponent>());
	white_cube_model.model = &AssetManager::GetModel("white_emissive_cube");

	cyanCube = createEntity();
	cyanCube->setPosition(Vector3{ 1.0f, 0.5f, 2.5f });
	cyanCube->setScale(0.2f);
	ModelRendererComponent& cyan_cube_model = ECS::GetComponent(cyanCube->addComponentByClass<ModelRendererComponent>());
	cyan_cube_model.model = &AssetManager::GetModel("cyan_emissive_cube");

	Entity* crate_1 = createEntity();
	crate_1->setPosition(Vector3{ 1.0f, 0.5f, 2.5f });
	ModelRendererComponent& crate_1_model = ECS::GetComponent(crate_1->addComponentByClass<ModelRendererComponent>());
	crate_1_model.model = &AssetManager::GetModel("container");

	Entity* crate_2 = createEntity();
	crate_2->setPosition(Vector3{ 2.0f, 0.5f, -1.5f });
	ModelRendererComponent& crate_2_model = ECS::GetComponent(crate_2->addComponentByClass<ModelRendererComponent>());
	crate_2_model.model = &AssetManager::GetModel("container");

	Entity* backpack = createEntity();
	backpack->setPosition(Vector3{ -1.0f, 0.5f, -2.5f });
	backpack->setScale(0.002f);
	ModelRendererComponent& backpack_model = ECS::GetComponent(backpack->addComponentByClass<ModelRendererComponent>());
	backpack_model.model = &AssetManager::GetModel("backpack");


	// Lights

	Entity* dir_light_entity = createEntity();
	DirectionalLightComponent& directional_light = ECS::GetComponent(dir_light_entity->addComponentByClass<DirectionalLightComponent>());
	directional_light.lightColor = Color::white;
	directional_light.direction = Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f });
	directional_light.ambientStrength = 0.2f;
	directional_light.diffuseStrength = 0.9f;

	PointLightComponent& white_point_light = ECS::GetComponent(white_cube->addComponentByClass<PointLightComponent>());
	white_point_light.lightColor = Color::white;

	PointLightComponent& cyan_point_light = ECS::GetComponent(cyanCube->addComponentByClass<PointLightComponent>());
	cyan_point_light.lightColor = Color::cyan;
	cyan_point_light.useColorToSpecular = true;

	Entity* spot_light_entity = createEntity();
	spot_light_entity->setPosition(Vector3{ -1.0f, 2.0f, -2.5f });
	SpotLightComponent& spot_light = ECS::GetComponent(spot_light_entity->addComponentByClass<SpotLightComponent>());
	spot_light.lightColor = Color::magenta;
	spot_light.direction = Vector3::negUnitY;
	spot_light.cutoffs = SpotLightCutoffs{ Maths::cos(Maths::toRadians(20.0f)), Maths::cos(Maths::toRadians(25.0f)) };
	spot_light.diffuseStrength = 1.0f;


	// Cameras

	Entity* cam_one_entity = createEntity();
	cam_one_entity->setPosition(Vector3{ 2.0f, 3.0f, -7.0f });
	cameraOne = cam_one_entity->addComponentByClass<CameraComponent>();
	ECS::GetComponent(cameraOne).setYaw(-110.0f);
	ECS::GetComponent(cameraOne).setPitch(-20.0f);

	Entity* cam_two_entity = createEntity();
	cam_two_entity->setPosition(Vector3{ -5.0f, 2.0f, 5.0f });
	cameraTwo = cam_two_entity->addComponentByClass<CameraComponent>();
	ECS::GetComponent(cameraTwo).setYaw(45.0f);
	ECS::GetComponent(cameraTwo).setPitch(-10.0f);
	
	ECS::GetComponent(cameraOne).setAsActiveCamera();
	activeCamera = cameraOne;
}

void WipScene::unloadScene()
{
}

void WipScene::updateScene(float dt)
{
	// Update cyan cube position
	cyanCubeTimer += dt;
	cyanCube->setPosition(Vector3{ 1.0f, 0.5f, 2.5f } + Vector3{ Maths::sin(cyanCubeTimer) * 1.2f, 0.0f, Maths::cos(cyanCubeTimer) * 1.2f });

	// Move active camera
	CameraComponent& cam_comp = ECS::GetComponent(activeCamera);
	Entity* cam_entity = cam_comp.getOwner();

	if (Input::IsKeyDown(GLFW_KEY_W))
		cam_entity->addPosition(cam_comp.getCamForward() * cameraSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_S))
		cam_entity->addPosition(-cam_comp.getCamForward() * cameraSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_A))
		cam_entity->addPosition(cam_comp.getCamRight() * cameraSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_D))
		cam_entity->addPosition(-cam_comp.getCamRight() * cameraSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_SPACE))
		cam_entity->addPosition(Vector3::unitY * cameraSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_C))
		cam_entity->addPosition(Vector3::negUnitY * cameraSpeed * dt);

	const Vector2 mouse_delta = Input::GetMouseDelta() * cameraSensitivity;
	cam_comp.addYaw(-mouse_delta.x);
	cam_comp.setPitch(Maths::clamp(cam_comp.getPitch() + mouse_delta.y, -89.0f, 89.0f));

	// Switch active camera
	if (Input::IsKeyDown(GLFW_KEY_KP_1))
	{
		ECS::GetComponent(cameraOne).setAsActiveCamera();
		activeCamera = cameraOne;
	}

	if (Input::IsKeyDown(GLFW_KEY_KP_2))
	{
		ECS::GetComponent(cameraTwo).setAsActiveCamera();
		activeCamera = cameraTwo;
	}
}
