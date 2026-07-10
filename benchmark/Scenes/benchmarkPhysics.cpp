#include "benchmarkPhysics.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>


void BenchmarkPhysics::loadScene()
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
	floor->setScale(80.0f);
	ModelRendererComponent& floor_model = ECS::GetComponent(floor->addComponentByClass<ModelRendererComponent>());
	floor_model.setModel(&AssetManager::GetModel("default_plane"));
	floor_model.setMaterial(&AssetManager::GetMaterial("floor"), 0);
	BoxCollisionComponent& floor_col_comp = ECS::GetComponent(floor->addComponentByClass<BoxCollisionComponent>());
	floor_col_comp.collisionBox = Box{ Vector3{ 0.0f, -0.05f, 0.0f }, Vector3{ 0.5f, 0.05f, 0.5f } };
	floor_col_comp.collisionChannel = "solid";

	// Create 100 falling bricks
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			Entity* brick = createEntity();
			brick->setPosition(Vector3{ (x - 4.5f) * 8.0f, 2.0f + (10 - z) * 1.2f, (z - 4.5f) * 8.0f });
			brick->setScale(0.2f);
			ECS::GetComponent(brick->addComponentByClass<ModelRendererComponent>()).setModel(&AssetManager::GetModel("orangebrick"));

			ComponentHandle<BoxCollisionComponent> brick_col_handle = brick->addComponentByClass<BoxCollisionComponent>();
			BoxCollisionComponent& brick_col_comp = ECS::GetComponent(brick_col_handle);
			brick_col_comp.collisionBox = Box{ Vector3::zero, Vector3{ 2.3f, 0.55f, 1.1f } };
			brick_col_comp.useEntityScaleForBoxCenter = false;
			brick_col_comp.useEntityScaleForBoxSize = false;
			brick_col_comp.collisionChannel = "solid";

			RigidbodyComponent& brick_rigidbody = ECS::GetComponent(brick->addComponentByClass<RigidbodyComponent>());
			brick_rigidbody.associateCollision(brick_col_handle);
			brick_rigidbody.collisionChannels = { "solid" };
		}
	}
}

void BenchmarkPhysics::unloadScene()
{
}

void BenchmarkPhysics::updateScene(float dt)
{
}