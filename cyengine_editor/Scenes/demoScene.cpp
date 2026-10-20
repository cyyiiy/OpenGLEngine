#include "demoScene.h"
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>

#include <Assets/assetManager.h>
#include <Rendering/Model/model.h>
#include <Rendering/material.h>
#include <Utils/Color.h>


void DemoScene::loadScene()
{
	const Vector3 cam_pos = Vector3{ 10.0f, 5.0f, 5.0f };

	Entity* cam_entity = createEntity();
	cam_entity->setPosition(cam_pos);
	cam_entity->setRotation(Quaternion::createLookAt(cam_pos, Vector3::zero, Vector3::unitY));
	CameraComponent& camera_comp = ECS::GetComponent(cam_entity->addComponentByClass<CameraComponent>());
	camera_comp.setYaw(-90.0f);
	camera_comp.setAsActiveCamera();


	Entity* cube_entity = createEntity();
	ModelRendererComponent& cube_model_comp = ECS::GetComponent(cube_entity->addComponentByClass<ModelRendererComponent>());
	cube_model_comp.setModel(AssetManager::GetAsset<Model>("default_cube"));
	cube_model_comp.setMaterial(AssetManager::GetAsset<Material>("default_grey_lit"), 0);

	Entity* light_entity = createEntity();
	DirectionalLightComponent& dir_light_comp = ECS::GetComponent(light_entity->addComponentByClass<DirectionalLightComponent>());
	dir_light_comp.direction = Vector3::normalize(Vector3{ -0.3f, -1.0f, -0.2f });
	dir_light_comp.lightColor = Color::cyan;
	dir_light_comp.diffuseStrength = 2.0f;
}

void DemoScene::unloadScene()
{}