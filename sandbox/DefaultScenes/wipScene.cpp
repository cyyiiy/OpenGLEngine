#include "wipScene.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Inputs/Input.h>
#include <Rendering/modelRendererComponent.h>


void WipScene::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	Entity* cube_1 = createEntity();
	cube_1->setPosition(Vector3::zero);
	cube_1->setScale(0.2f);
	ModelRendererComponent& cube_1_model = ECS::GetComponent(cube_1->addComponentByClass<ModelRendererComponent>());
	cube_1_model.model = &AssetManager::GetModel("white_emissive_cube");

	Entity* cube_2 = createEntity();
	cube_2->setPosition(Vector3::unitX);
	cube_2->setScale(0.2f);
	ModelRendererComponent& cube_2_model = ECS::GetComponent(cube_2->addComponentByClass<ModelRendererComponent>());
	cube_2_model.model = &AssetManager::GetModel("white_emissive_cube");

	Entity* cube_3 = createEntity();
	cube_3->setPosition(Vector3::unitY);
	cube_3->setScale(0.2f);
	ModelRendererComponent& cube_3_model = ECS::GetComponent(cube_3->addComponentByClass<ModelRendererComponent>());
	cube_3_model.model = &AssetManager::GetModel("white_emissive_cube");

	Entity* cube_4 = createEntity();
	cube_4->setPosition(Vector3::unitZ);
	cube_4->setScale(0.2f);
	ModelRendererComponent& cube_4_model = ECS::GetComponent(cube_4->addComponentByClass<ModelRendererComponent>());
	cube_4_model.model = &AssetManager::GetModel("white_emissive_cube");

	Entity* cam_one_entity = createEntity();
	cam_one_entity->setPosition(Vector3{ -8.0f, 3.0f, -4.0f });
	cam_one_entity->setRotation(Quaternion::createLookAt(cam_one_entity->getPosition(), Vector3::zero, Vector3::unitY));
	cameraOne = cam_one_entity->addComponentByClass<CameraComponent>();
	ECS::GetComponent(cameraOne).setYaw(-90.0f);

	Entity* cam_two_entity = createEntity();
	cam_two_entity->setPosition(Vector3{ 1.0f, -2.0f, 7.0f });
	cam_two_entity->setRotation(Quaternion::createLookAt(cam_two_entity->getPosition(), Vector3::zero, Vector3::unitY));
	cameraTwo = cam_two_entity->addComponentByClass<CameraComponent>();
	ECS::GetComponent(cameraTwo).setYaw(-90.0f);
	
	ECS::GetComponent(cameraOne).setAsActiveCamera();
}

void WipScene::unloadScene()
{
}

void WipScene::updateScene(float dt)
{
	if (Input::IsKeyDown(GLFW_KEY_KP_1))
	{
		ECS::GetComponent(cameraOne).setAsActiveCamera();
	}

	if (Input::IsKeyDown(GLFW_KEY_KP_2))
	{
		ECS::GetComponent(cameraTwo).setAsActiveCamera();
	}
}
