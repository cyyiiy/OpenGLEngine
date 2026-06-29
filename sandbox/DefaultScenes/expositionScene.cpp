#include "expositionScene.h"
#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Inputs/Input.h>
#include <Maths/Geometry/box.h>
#include <Core/Debug/debugManager.h>

#include <Rendering/cameraComponent.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/Lights/spotLightComponent.h>
#include <Rendering/Text/textComponent.h>
#include <Rendering/Hud/spriteComponent.h>
#include <Audio/audioSourceComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/rigidbodyComponent.h>


void ExpositionScene::loadScene()
{
	Renderer& renderer = Locator::getRenderer();
	renderer.SetClearColor(Color{ 50, 75, 75, 255 });

	// Models

	Entity* floor = createEntity();
	floor->setScale(10.0f);
	ModelRendererComponent& floor_model = ECS::GetComponent(floor->addComponentByClass<ModelRendererComponent>());
	floor_model.model = &AssetManager::GetModel("floor_wood");

	whiteCube = createEntity();
	whiteCube->setPosition(Vector3{ -2.0f, 0.4f, 1.5f });
	whiteCube->setScale(0.2f);
	ModelRendererComponent& white_cube_model = ECS::GetComponent(whiteCube->addComponentByClass<ModelRendererComponent>());
	white_cube_model.model = &AssetManager::GetModel("white_emissive_cube");

	cyanCube = createEntity();
	cyanCube->setPosition(Vector3{ 1.0f, 0.5f, 2.5f });
	cyanCube->setScale(0.2f);
	ModelRendererComponent& cyan_cube_model = ECS::GetComponent(cyanCube->addComponentByClass<ModelRendererComponent>());
	cyan_cube_model.model = &AssetManager::GetModel("cyan_emissive_cube");

	Entity* crate = createEntity();
	crate->setPosition(Vector3{ 1.0f, 0.5f, 2.5f });
	ModelRendererComponent& crate_model = ECS::GetComponent(crate->addComponentByClass<ModelRendererComponent>());
	crate_model.model = &AssetManager::GetModel("container");

	gravityCrate = createEntity();
	gravityCrate->setPosition(Vector3{ 2.0f, 5.0f, -1.5f });
	ModelRendererComponent& gravity_crate_model = ECS::GetComponent(gravityCrate->addComponentByClass<ModelRendererComponent>());
	gravity_crate_model.model = &AssetManager::GetModel("container");

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

	PointLightComponent& white_point_light = ECS::GetComponent(whiteCube->addComponentByClass<PointLightComponent>());
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


	// Hud

	Entity* hud_entity = createEntity();

	sprite = hud_entity->addComponentByClass<SpriteComponent>();
	SpriteComponent& sprite_comp = ECS::GetComponent(sprite);
	sprite_comp.texture = &AssetManager::GetTexture("smileyface_sprite");
	sprite_comp.position = HudPosition{ Vector2{ 1.0f, 0.0f }, Vector2::halfUnit, Vector2{ -80.0f, 80.0f } };
	sprite_comp.scale = Vector2{ 0.25f };

	TextComponent& text_comp = ECS::GetComponent(hud_entity->addComponentByClass<TextComponent>());
	text_comp.setTextDatas("Sandbox project", AssetManager::GetFont("octosquares_32"));
	text_comp.position = HudPosition{ Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f }, Vector2{ 20.0f, 20.0f } };

	camText = hud_entity->addComponentByClass<TextComponent>();
	TextComponent& cam_text_comp = ECS::GetComponent(camText);
	cam_text_comp.setTextDatas("Using camera 1", AssetManager::GetFont("octosquares_32"));
	cam_text_comp.position = HudPosition{ Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.0f }, Vector2{ 20.0f, 60.0f } };


	// Audio

	musicSource = backpack->addComponentByClass<AudioSourceComponent>();
	AudioSourceComponent& music_source_comp = ECS::GetComponent(musicSource);
	music_source_comp.playSound(AssetManager::GetSound("music"), -1);
	music_source_comp.setPause(true);


	// Physics

	BoxCollisionComponent& crate_col = ECS::GetComponent(crate->addComponentByClass<BoxCollisionComponent>());
	crate_col.collisionChannel = "solid";

	BoxCollisionComponent& gravity_crate_col = ECS::GetComponent(gravityCrate->addComponentByClass<BoxCollisionComponent>());
	gravity_crate_col.collisionChannel = "solid";
	RigidbodyComponent& gravity_crate_rigidbody = ECS::GetComponent(gravityCrate->addComponentByClass<RigidbodyComponent>());
	gravity_crate_rigidbody.associateCollision(gravityCrate->getComponentOfClass<BoxCollisionComponent>());

	BoxCollisionComponent& floor_col = ECS::GetComponent(floor->addComponentByClass<BoxCollisionComponent>());
	floor_col.collisionBox = Box{ Vector3{ 0.0f, -0.01f, 0.0f }, Vector3{ 0.5f, 0.01f, 0.5f } };
	floor_col.collisionChannel = "solid";
}

void ExpositionScene::unloadScene()
{
}

void ExpositionScene::updateScene(float dt)
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
	if (Input::IsKeyPressed(GLFW_KEY_KP_1))
	{
		ECS::GetComponent(cameraOne).setAsActiveCamera();
		activeCamera = cameraOne;
		ECS::GetComponent(camText).setText("Using camera 1");
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_2))
	{
		ECS::GetComponent(cameraTwo).setAsActiveCamera();
		activeCamera = cameraTwo;
		ECS::GetComponent(camText).setText("Using camera 2");
	}

	// Write log on screen message
	if (Input::IsKeyPressed(GLFW_KEY_KP_3))
	{
		Locator::getLog().LogMessageToScreen("Log test message number " + std::to_string(++logCounter), Color::cyan, 5.0f);
	}

	// Draw debug shapes
	if (Input::IsKeyPressed(GLFW_KEY_KP_4))
	{
		ComponentHandle<CameraComponent> inactive_camera = (activeCamera == cameraOne) ? cameraTwo : cameraOne;
		DebugManager::DrawDebugPoint(ECS::GetComponent(inactive_camera).getCamPosition(), Color::red, 5.0f);
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_5))
	{
		ComponentHandle<CameraComponent> inactive_camera = (activeCamera == cameraOne) ? cameraTwo : cameraOne;
		CameraComponent& cam = ECS::GetComponent(inactive_camera);
		DebugManager::DrawDebugLine(cam.getCamPosition(), cam.getCamPosition() + cam.getCamForward() * 2.0f, Color::magenta, 5.0f);
	}

	if (Input::IsKeyPressed(GLFW_KEY_KP_6))
	{
		Box debug_box{ cyanCube->getPosition(), Vector3::one * 0.1f };
		DebugManager::DrawDebugCube(debug_box, Color::blue, 5.0f);
	}

	// Play 2D sound (without audio source component)
	if (Input::IsKeyPressed(GLFW_KEY_KP_7))
	{
		Locator::getAudio().InstantPlaySound2D(AssetManager::GetSound("sound"), 0.2f);
	}

	// Pause (and play) music on audio source component
	if (Input::IsKeyPressed(GLFW_KEY_KP_8))
	{
		AudioSourceComponent& music_source_comp = ECS::GetComponent(musicSource);
		music_source_comp.setPause(!music_source_comp.getPaused());
	}

	// Move the gravity crate upwards
	if (Input::IsKeyPressed(GLFW_KEY_KP_9))
	{
		gravityCrate->addPosition(Vector3::unitY * 5.0f);
	}

	// Shoot a line raycast in front of the camera
	if (Input::IsKeyPressed(GLFW_MOUSE_BUTTON_1))
	{
		Physics& physics = Locator::getPhysics();
		const CameraComponent& camera = ECS::GetComponent(activeCamera);

		const Vector3 raycast_end_pos = camera.getCamPosition() + camera.getCamForward() * 50.0f;
		physics.LineRaycast(camera.getCamPosition(), raycast_end_pos, { "solid" });
	}

	// Shoot an AABB sweep raycast between the two light cubes
	if (Input::IsKeyPressed(GLFW_MOUSE_BUTTON_2))
	{
		Physics& physics = Locator::getPhysics();
		physics.AABBSweepRaycast(whiteCube->getPosition(), cyanCube->getPosition(), Box{ Vector3::zero, Vector3::one * 0.1f }, { "solid" });
	}

	// Automatic rotation of the sprite
	SpriteComponent& sprite_comp = ECS::GetComponent(sprite);
	sprite_comp.rotAngle = Maths::fmod(sprite_comp.rotAngle + dt * 180.0f, 360.0f);
}