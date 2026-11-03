#include "demoScene.h"
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <EditorUtils/editorStatics.h>

#include <Assets/assetManager.h>
#include <Rendering/Model/model.h>
#include <Rendering/material.h>
#include <Utils/Color.h>
#include <imgui/imgui.h>

const float CAMERA_SPEED = 4.0f;
const float CAMERA_MOUSE_SENSITIVITY = 0.1f;

void DemoScene::loadScene()
{
	const Vector3 cam_pos = Vector3{ 10.0f, 5.0f, 5.0f };

	cameraEntity = createEntity();
	cameraEntity->setPosition(cam_pos);
	cameraEntity->setRotation(Quaternion::createLookAt(cam_pos, Vector3::zero, Vector3::unitY));
	cameraComp = cameraEntity->addComponentByClass<CameraComponent>();
	CameraComponent& camera_comp = ECS::GetComponent(cameraComp);
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

void DemoScene::updateScene(float dt)
{
	if (EditorStatics::isViewportFocused)
	{
		CameraComponent& camera_comp = ECS::GetComponent(cameraComp);

		if (ImGui::IsKeyDown(ImGuiKey_Z))
			cameraEntity->addPosition(camera_comp.getCamForward() * CAMERA_SPEED * dt);

		if (ImGui::IsKeyDown(ImGuiKey_S))
			cameraEntity->addPosition(-camera_comp.getCamForward() * CAMERA_SPEED * dt);

		if (ImGui::IsKeyDown(ImGuiKey_Q))
			cameraEntity->addPosition(camera_comp.getCamRight() * CAMERA_SPEED * dt);

		if (ImGui::IsKeyDown(ImGuiKey_D))
			cameraEntity->addPosition(-camera_comp.getCamRight() * CAMERA_SPEED * dt);

		if (ImGui::IsKeyDown(ImGuiKey_Space))
			cameraEntity->addPosition(Vector3::unitY * CAMERA_SPEED * dt);

		if (ImGui::IsKeyDown(ImGuiKey_C))
			cameraEntity->addPosition(Vector3::negUnitY * CAMERA_SPEED * dt);

		ImGuiIO& io = ImGui::GetIO();

		const Vector2 mouse_delta = Vector2{ io.MouseDelta.x, io.MouseDelta.y } * CAMERA_MOUSE_SENSITIVITY;
		camera_comp.addYaw(mouse_delta.x);
		camera_comp.setPitch(Maths::clamp(camera_comp.getPitch() - mouse_delta.y, -89.0f, 89.0f));
	}
}