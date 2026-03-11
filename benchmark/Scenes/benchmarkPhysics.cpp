#include "benchmarkPhysics.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Physics/AABB/boxAABBColComp.h>
#include <Physics/rigidbodyComponent.h>


void BenchmarkPhysics::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Initialize camera
	Entity* camera = createEntity();
	camera->addComponentByClass<CameraComponent>()->setAsActiveCamera();
	camera->getComponentByClass<CameraComponent>()->setYaw(-90.0f);
	camera->setPosition(Vector3{ -1.0f * 60.0f, 40.0f, 60.0f });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), Vector3{ 0.0f, 1.0f, 0.0f }, Vector3::unitY));

	// Create directional light
	Entity* dir_light = createEntity();
	std::shared_ptr<DirectionalLightComponent> dir_light_comp = dir_light->addComponentByClass<DirectionalLightComponent>();
	dir_light_comp->setColor(Color::white);
	dir_light_comp->setDirection(Vector3::normalize(Vector3{ 0.5f, -1.0f, 0.75f }));
	dir_light_comp->setAmbientStrength(0.1f);
	dir_light_comp->setDiffuseStrength(0.7f);

	// Create floor
	Entity* floor = createEntity();
	floor->setScale(80.0f);
	floor->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel("floor"));
	std::shared_ptr<BoxAABBColComp> floor_col_comp = floor->addComponentByClass<BoxAABBColComp>();
	floor_col_comp->setBox(Box{ Vector3{ 0.0f, -0.05f, 0.0f }, Vector3{ 0.5f, 0.05f, 0.5f } });
	floor_col_comp->setCollisionChannel("solid");

	// Create 100 falling bricks
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			Entity* brick = createEntity();
			brick->setPosition(Vector3{ (x - 4.5f) * 8.0f, 2.0f + (10 - z) * 1.2f, (z - 4.5f) * 8.0f });
			brick->setScale(0.2f);
			brick->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel("orangebrick"));

			std::shared_ptr<BoxAABBColComp> brick_col_comp = brick->addComponentByClass<BoxAABBColComp>();
			brick_col_comp->setBox(Box{ Vector3::zero, Vector3{ 2.3f, 0.55f, 1.1f } });
			brick_col_comp->useOwnerScaleForBoxCenter = false;
			brick_col_comp->useOwnerScaleForBoxSize = false;
			brick_col_comp->setCollisionChannel("solid");

			std::shared_ptr<RigidbodyComponent> brick_rigidbody = brick->addComponentByClass<RigidbodyComponent>();
			brick_rigidbody->associateCollision(brick_col_comp);
			brick_rigidbody->setPhysicsActivated(true);
			brick_rigidbody->setUseGravity(true);
			brick_rigidbody->setTestChannels({ "solid" });
		}
	}
}

void BenchmarkPhysics::unloadScene()
{
}

void BenchmarkPhysics::updateScene(float dt)
{
}