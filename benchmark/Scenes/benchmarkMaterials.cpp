#include "benchmarkMaterials.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <sstream>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>


const Vector3 CAMERA_TARGET = Vector3::zero;
const float CAMERA_HEIGHT = 25.0f;
const float CAMERA_RADIUS = 50.0f;
const float CAMERA_SPEED = 0.3f;


void BenchmarkMaterials::loadScene()
{
	// Create 729 unique materials & models
	// =========================================
	for (int r = 0; r < 9; r++)
	{
		for (int g = 0; g < 9; g++)
		{
			for (int b = 0; b < 9; b++)
			{
				std::stringstream name_suffix;
				name_suffix << "_" << r << "_" << g << "_" << b;

				const std::string material_name = "material" + name_suffix.str();
				const Color material_color = Color{ 10 + 29 * r, 10 + 29 * g, 10 + 29 * b, 255 };
				Material& material = AssetManager::CreateMaterial(material_name, AssetManager::GetShader("flat_emissive"));
				material.addParameter("emissive", material_color.toVector());

				const std::string model_name = "model" + name_suffix.str();
				AssetManager::CreateModel(model_name).addMesh(AssetManager::GetSingleMesh("default_cube"), material);
			}
		}
	}


	// Create benchmark scene
	//==========================================
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Initialize camera
	camera = createEntity();
	camera->addComponentByClass<CameraComponent>()->setAsActiveCamera();
	camera->getComponentByClass<CameraComponent>()->setYaw(-90.0f);
	camera->setPosition(Vector3{ -1.0f * CAMERA_RADIUS, CAMERA_HEIGHT, 0.0f });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), CAMERA_TARGET, Vector3::unitY));

	// Create 729 cubes
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			for (int z = 0; z < 9; z++)
			{
				Entity* cube = createEntity();
				cube->setPosition(Vector3{ (x - 4) * 4.0f, (y - 4) * 4.0f, (z - 4) * 4.0f });

				std::stringstream model_name;
				model_name << "model_" << x << "_" << y << "_" << z;
				cube->addComponentByClass<ModelRendererComponent>()->setModel(&AssetManager::GetModel(model_name.str()));
			}
		}
	}
}

void BenchmarkMaterials::unloadScene()
{
	// Delete the materials
	for (int r = 0; r < 9; r++)
	{
		for (int g = 0; g < 9; g++)
		{
			for (int b = 0; b < 9; b++)
			{
				std::stringstream name_suffix;
				name_suffix << "_" << r << "_" << g << "_" << b;

				const std::string material_name = "material" + name_suffix.str();
				const std::string model_name = "model" + name_suffix.str();

				AssetManager::DeleteModel(model_name);
				AssetManager::DeleteMaterial(material_name);
			}
		}
	}
}

void BenchmarkMaterials::updateScene(float dt)
{
	// Rotate camera around scene center
	cameraTimer += dt * CAMERA_SPEED;
	camera->setPosition(Vector3{ Maths::cos(cameraTimer) * -1.0f * CAMERA_RADIUS, CAMERA_HEIGHT, Maths::sin(cameraTimer) * -1.0f * CAMERA_RADIUS });
	camera->setRotation(Quaternion::createLookAt(camera->getPosition(), CAMERA_TARGET, Vector3::unitY));
}
