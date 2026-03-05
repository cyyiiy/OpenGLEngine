#include "benchmarkRendering3D.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>


const Vector3 CAMERA_TARGET = Vector3{ 0.0f, 1.0f, 0.0f };
const float CAMERA_HEIGHT = 30.0f;
const float CAMERA_RADIUS = 60.0f;
const float CAMERA_SPEED = 0.3f;


void BenchmarkRendering3D::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Initialize camera
	camera = createEntity();
	camera->addComponentByClass<CameraComponent>()->setAsActiveCamera();
	camera->getComponentByClass<CameraComponent>()->setYaw(-90.0f);
	camera->setPosition(Vector3{ -1.0f * CAMERA_RADIUS, CAMERA_HEIGHT, 0.0f });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), CAMERA_TARGET, Vector3::unitY));

	// Create directional light
	Entity* dir_light = createEntity();
	std::shared_ptr<DirectionalLightComponent> dir_light_comp = dir_light->addComponentByClass<DirectionalLightComponent>();
	dir_light_comp->setColor(Color::white);
	dir_light_comp->setDirection(Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f }));
	dir_light_comp->setAmbientStrength(0.1f);
	dir_light_comp->setDiffuseStrength(0.7f);

	// Create floor
	Entity* floor = createEntity();
	floor->setScale(100.0f);
	floor->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel("floor"));

	// Create 400 wooden chests
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			Entity* chest = createEntity();
			chest->setPosition(Vector3{ (x - 9.5f) * 5.0f, 1.0f, (z - 9.5f) * 5.0f });
			chest->setScale(0.1f);
			chest->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel("woodenchest"));
		}
	}

	// Create 81 statues
	for (int x = 0; x < 9; x++)
	{
		for (int z = 0; z < 9; z++)
		{
			Entity* statue = createEntity();
			statue->setPosition(Vector3{ (x - 4) * 10.0f, 0.0f, (z - 4) * 10.0f });
			statue->setScale(0.03f);
			statue->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel("romanstatue"));
		}
	}

	// Create 64 point lights
	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			Entity* point_light = createEntity();
			point_light->setPosition(Vector3{ (x - 3.5f) * 10.0f, 3.0f, (z - 3.5f) * 10.0f });
			std::shared_ptr<PointLightComponent> point_light_comp = point_light->addComponentByClass<PointLightComponent>();
			point_light_comp->setDiffuseStrength(1.0f);
			point_light_comp->setAmbientStrength(0.0f);
			point_light_comp->setUseDiffColorToSpecColor(true);
			point_light_comp->setFalloff(1.0f, 0.045f, 0.0075f);

			const int color_id = (x + z) % 4;
			switch (color_id)
			{
			case 0:
				point_light_comp->setColor(Color::red);
				break;
			case 1:
				point_light_comp->setColor(Color::green);
				break;
			case 2:
				point_light_comp->setColor(Color::blue);
				break;
			case 3:
				point_light_comp->setColor(Color::yellow);
				break;
			}
		}
	}
}

void BenchmarkRendering3D::unloadScene()
{
}

void BenchmarkRendering3D::updateScene(float dt)
{
	// Rotate camera around scene center
	cameraTimer += dt * CAMERA_SPEED;
	camera->setPosition(Vector3{ Maths::cos(cameraTimer) * -1.0f * CAMERA_RADIUS, CAMERA_HEIGHT, Maths::sin(cameraTimer) * -1.0f * CAMERA_RADIUS });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), CAMERA_TARGET, Vector3::unitY));
}
