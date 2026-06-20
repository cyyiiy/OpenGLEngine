#include "debugManager.h"
#include <ECS/ecs.h>
#include <Inputs/Input.h>
#include <ECS/entityContainer.h>
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <Rendering/cameraComponent.h>
#include <Rendering/Text/textComponent.h>
#include <sstream>
#include <iomanip>


bool DebugManager::pause;
bool DebugManager::oneFrame;
bool DebugManager::freecam;
float DebugManager::freecamSpeed;
bool DebugManager::debugView;

int DebugManager::fpsCounter;
float DebugManager::fpsTimeCounter;
std::string DebugManager::currentFpsText;

class Entity* DebugManager::freeCameraEntity;
ComponentHandle<class CameraComponent> DebugManager::freeCamera;
ComponentHandle<class TextComponent> DebugManager::debugInfoText;


void DebugManager::InitializeDebugManager(EntityContainer& entityOwner)
{
	// Initialize the variables
	pause = false;
	oneFrame = false;
	freecam = false;
	freecamSpeed = DebugConsts::FREECAM_SLOW_SPEED;
	debugView = false;
	fpsCounter = 0;
	fpsTimeCounter = 0.0f;
	currentFpsText = "FPS : 0 - (0.00 ms)";

	// Create the free camera
	freeCameraEntity = entityOwner.createEntity();
	freeCamera = freeCameraEntity->addComponentByClass<CameraComponent>();
	Locator::getRenderer().SetDebugCamera(freeCamera);

	// Create the fps text
	debugInfoText = entityOwner.createEntity()->addComponentByClass<TextComponent>();
	TextComponent& fps_text_comp = ECS::GetComponent(debugInfoText);
	fps_text_comp.setTextDatas("", AssetManager::GetFont("arial_24"));
	fps_text_comp.position = HudPosition{ Vector2::one, Vector2::one, Vector2{ -20.0f, -20.0f } };
	fps_text_comp.active = false;
}

void DebugManager::UpdateDebugManager(float dt)
{
	// Process debug inputs only if not compiled in release mode
#if !defined NDEBUG
	ProcessDebugInputs();
#endif

	// Process free camera inputs
	if (freecam) ProcessFreecamInputs(dt);

	// Update the debug info text
	if (pause || debugView) UpdateDebugInfoText(dt);
}

void DebugManager::ProcessDebugInputs()
{
	// 'P' key (azerty) -> pause game
	if (Input::IsKeyPressed(GLFW_KEY_P))
	{
		if (!pause) PauseGame();
		else UnpauseGame();
	}

	// 'O' key (azerty) -> one frame advance
	if (Input::IsKeyPressed(GLFW_KEY_O))
	{
		AdvanceOneFrame();
	}

	// 'M' key (azerty) -> freecam
	if (Input::IsKeyPressed(GLFW_KEY_SEMICOLON))
	{
		if (!freecam) EnableFreecam();
		else DisableFreecam();
	}

	// 'K' key (azerty) -> debug view
	if (Input::IsKeyPressed(GLFW_KEY_K))
	{
		if (!debugView) EnableDebugView();
		else DisableDebugView();
	}
}

void DebugManager::ProcessFreecamInputs(float dt)
{
	CameraComponent& free_camera_comp = ECS::GetComponent(freeCamera);

	if (Input::IsKeyDown(GLFW_KEY_W))
		freeCameraEntity->addPosition(free_camera_comp.getCamForward() * freecamSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_S))
		freeCameraEntity->addPosition(-free_camera_comp.getCamForward() * freecamSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_A))
		freeCameraEntity->addPosition(free_camera_comp.getCamRight() * freecamSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_D))
		freeCameraEntity->addPosition(-free_camera_comp.getCamRight() * freecamSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_SPACE))
		freeCameraEntity->addPosition(Vector3::unitY * freecamSpeed * dt);

	if (Input::IsKeyDown(GLFW_KEY_C))
		freeCameraEntity->addPosition(Vector3::negUnitY * freecamSpeed * dt);

	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		freecamSpeed = 10.0f;

	if (Input::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		freecamSpeed = 4.0f;

	const Vector2 mouse_delta = Input::GetMouseDelta() * DebugConsts::FREECAM_MOUSE_SENSITIVITY;
	free_camera_comp.addYaw(-mouse_delta.x);
	free_camera_comp.setPitch(Maths::clamp(free_camera_comp.getPitch() + mouse_delta.y, -89.0f, 89.0f));

	const float scroll_offset = Input::GetScrollOffset();
	free_camera_comp.setFov(Maths::clamp(free_camera_comp.getFov() - scroll_offset, 1.0f, 45.0f));
}

void DebugManager::UpdateDebugInfoText(float dt)
{
	if (!pause && !debugView)
	{
		ECS::GetComponent(debugInfoText).active = false;
		return;
	}

	// 1. Setup
	TextComponent& debug_info_text_comp = ECS::GetComponent(debugInfoText);
	debug_info_text_comp.active = true;

	if (dt == 0.0f) return;
	std::stringstream debug_text_stream;

	// 2. Compute the fps (if debug view is enabled)
	if (debugView)
	{
		fpsCounter++;
		fpsTimeCounter += dt;
		if (fpsTimeCounter >= 1.0f)
		{
			const float frame_ms = 1000.0f / fpsCounter;
			std::stringstream ms_stream;
			ms_stream << std::fixed << std::setprecision(2) << frame_ms;
			debug_text_stream << "FPS: " << fpsCounter << " - (" << ms_stream.str() << " ms)";
			currentFpsText = debug_text_stream.str();

			fpsTimeCounter -= 1.0f;
			fpsCounter = 0;
		}
		else
		{
			debug_text_stream << currentFpsText;
		}
		debug_text_stream << "\n";
	}

	// 3. Add the info texts
	if (pause)
	{
		debug_text_stream << "Game paused\n";
	}
	if (freecam)
	{
		debug_text_stream << "Freecam enabled\n";
	}
	if (debugView)
	{
		debug_text_stream << "Debug view enabled\n";
	}

	// 4. Apply the text
	debug_info_text_comp.setText(debug_text_stream.str());
}


void DebugManager::PauseGame()
{
	pause = true;
	UpdateDebugInfoText(0);
	Locator::getLog().LogMessage_Category("Debug: Game paused", LogCategory::Info);
	Locator::getAudio().PauseAll();
}

void DebugManager::UnpauseGame()
{
	if (freecam) DisableFreecam();
	pause = false;
	UpdateDebugInfoText(0);
	Locator::getLog().LogMessage_Category("Debug: Game unpaused", LogCategory::Info);
	Locator::getAudio().ResumeAll();
}

void DebugManager::AdvanceOneFrame()
{
	if (!pause) return;
	oneFrame = true;
	Locator::getLog().LogMessage_Category("Debug: Advance one frame", LogCategory::Info);
}

void DebugManager::EnableFreecam()
{
	if (!pause) PauseGame();
	freecam = true;
	Locator::getLog().LogMessage_Category("Debug: Freecam mode enabled", LogCategory::Info);

	Renderer& renderer = Locator::getRenderer();
	ECS::GetComponent(freeCamera).copyCamera(renderer.GetCamera(), true);
	renderer.SetDebugCamActivated(true);
	freecamSpeed = DebugConsts::FREECAM_SLOW_SPEED;
}

void DebugManager::DisableFreecam()
{
	freecam = false;
	Locator::getLog().LogMessage_Category("Debug: Freecam mode disabled", LogCategory::Info);
	Locator::getRenderer().SetDebugCamActivated(false);
}

void DebugManager::EnableDebugView()
{
	debugView = true;
	UpdateDebugInfoText(0);
	Locator::getLog().LogMessage_Category("Debug: Debug mode view enabled", LogCategory::Info);
	Locator::getRenderer().SetDebugViewMode(true);
}

void DebugManager::DisableDebugView()
{
	debugView = false;
	UpdateDebugInfoText(0);
	Locator::getLog().LogMessage_Category("Debug: Debug mode view disabled", LogCategory::Info);
	Locator::getRenderer().SetDebugViewMode(false);
}


bool DebugManager::GetPauseState() noexcept
{
	return pause;
}

bool DebugManager::ConsumeOneFrameAdvance() noexcept
{
	if (!oneFrame) return false;

	oneFrame = false;
	return true;
}

bool DebugManager::GetFreecamState() noexcept
{
	return freecam;
}

bool DebugManager::GetDebugViewState() noexcept
{
	return debugView;
}