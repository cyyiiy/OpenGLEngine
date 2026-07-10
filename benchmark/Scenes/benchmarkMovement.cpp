#include "benchmarkMovement.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Components/benchmarkComponentMovement.h>


void BenchmarkMovement::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Initialize camera
	Entity* camera = createEntity();
	CameraComponent& camera_comp = ECS::GetComponent(camera->addComponentByClass<CameraComponent>());
	camera_comp.setAsActiveCamera();
	camera_comp.setYaw(-90.0f);
	camera->setPosition(Vector3{ -1.0f * 60.0f, 40.0f, 60.0f });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, Vector3::unitY));

	// Create directional light
	Entity* dir_light = createEntity();
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(dir_light->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.lightColor = Color::white;
	dir_light_comp.direction = Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f });
	dir_light_comp.ambientStrength = 0.1f;
	dir_light_comp.diffuseStrength = 0.7f;

	// Create floor
	Entity* floor = createEntity();
	floor->setScale(100.0f);
	ModelRendererComponent& floor_model = ECS::GetComponent(floor->addComponentByClass<ModelRendererComponent>());
	floor_model.setModel(&AssetManager::GetModel("default_plane"));
	floor_model.setMaterial(&AssetManager::GetMaterial("floor"), 0);

	// Create 400 wooden chests
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			const float timer = (x / 40.0f + z / 40.0f) * 3.0f;

			Entity* chest = createEntity();
			chest->setPosition(Vector3{ (x - 9.5f) * 5.0f, 0.0f, (z - 9.5f) * 5.0f });
			chest->setScale(0.1f);
			ECS::GetComponent(chest->addComponentByClass<ModelRendererComponent>()).setModel(&AssetManager::GetModel("woodenchest"));
			ECS::GetComponent(chest->addComponentByClass<BenchmarkComponentMovement>()).initializeTimer(timer, MovementMode::Wave);
		}
	}

	// Create 64 point lights
	for (int i = 0; i < 64; i++)
	{
		Entity* point_light = createEntity();
		const float timer = i / (Maths::pi * Maths::pi);
		point_light->setPosition(Vector3{ Maths::cos(timer) * 35.0f, 5.0f, Maths::sin(timer) * 35.0f });
		ECS::GetComponent(point_light->addComponentByClass<BenchmarkComponentMovement>()).initializeTimer(timer, MovementMode::Circle);

		PointLightComponent& point_light_comp = ECS::GetComponent(point_light->addComponentByClass<PointLightComponent>());
		point_light_comp.diffuseStrength = 1.0f;
		point_light_comp.ambientStrength = 0.0f;
		point_light_comp.useColorToSpecular = true;
		point_light_comp.attenuation = LightAttenuation{ 1.0f, 0.045f, 0.0075f };
		point_light_comp.lightColor = Color{ (i * 4) % 256, (85 + i * 4) % 256, (170 + i * 4) % 256, 255 };
	}
}

void BenchmarkMovement::unloadScene()
{
}

void BenchmarkMovement::updateScene(float dt)
{
}
