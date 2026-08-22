#include "engine.h"
#include <Assets/assetManager.h>
#include <Assets/engineAssets.h>

#include <GameplayStatics/gameplayStatics.h>
#include <Inputs/input.h>
#include <Core/Debug/debugManager.h>
#include <ECS/ecs.h>
#include <ECS/Gameplay/gameplayComponentsManager.h>

#include <ServiceLocator/locator.h>
#include <Rendering/rendererOpenGL.h>
#include <PhysicsAABB/physicsManager.h>
#include <Audio/audioManager.h>
#include <Log/logManager.h>

#include <iostream>
#include <chrono>


bool Engine::initialize(int wndw_width, int wndw_height, std::string wndw_name, bool wndw_capturemouse)
{
	std::cout << "Initializing...\n\n\n";

	std::cout << "==================================================" << std::endl;
	std::cout << "   ____            _____             _            " << std::endl;
	std::cout << "  / ___|   _      | ____|_ __   __ _(_)_ __   ___ " << std::endl;
	std::cout << " | |  | | | |_____|  _| | '_ \\ / _` | | '_ \\ / _ \\ " << std::endl;
	std::cout << " | |__| |_| |_____| |___| | | | (_| | | | | |  __/" << std::endl;
	std::cout << "  \\____\\__, |     |_____|_| |_|\\__, |_|_| |_|\\___|" << std::endl;
	std::cout << "       |___/                   |___/              " << std::endl;
	std::cout << "==================================================" << std::endl << std::endl << std::endl;


	// Create window and initialize glfw
	std::cout << "Initializing window...";
	window.createWindow(wndw_width, wndw_height, wndw_name, wndw_capturemouse);

	GLFWwindow* gl_window = window.getGLFWwindow();
	if (gl_window == NULL)
	{
		std::cout << std::endl << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	GameplayStatics::SetWindowSize(Vector2Int{ window.getWidth(), window.getHeigth() });
	std::cout << " Done.\n";


	glfwSetWindowUserPointer(gl_window, this);

	glfwSetFramebufferSizeCallback(gl_window, [](GLFWwindow* window, int width, int height)
		{
			auto self = static_cast<Engine*>(glfwGetWindowUserPointer(window));
			self->windowResize(window, width, height);
		}
	); // Link window resize callback function

	glfwSetCursorPosCallback(gl_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			Input::ProcessMouse(window, xpos, ypos);
		}
	); // Link mouse pos callback function

	glfwSetScrollCallback(gl_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Input::ProcessScroll(window, xoffset, yoffset);
		}
	); // Link mouse scroll callback function

	glfwSetKeyCallback(gl_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Input::ProcessKeyboard(window, key, scancode, action, mods);
		}
	); // Link keyboard callback function

	glfwSetMouseButtonCallback(gl_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Input::ProcessMouseButton(window, button, action, mods);
		}
	); // Link mouse button callback function


	//  initialize service locator
	std::cout << "Initializing service locator...";
	Locator::initialize();
	std::cout << " Done.\n";


	// Create log manager
	std::cout << "Initializing log...";
	Log& log = Locator::provideLog(std::make_shared<LogManager>());
	log.Initialize();
	std::cout << " Done.\n";


	// Create renderer and default camera
	std::cout << "Initializing renderer...";
	Entity* default_cam_entity = createEntity();
	default_cam_entity->addComponentByClass<CameraComponent>();
	Renderer& renderer = Locator::provideRenderer(std::make_shared<RendererOpenGL>());
	renderer.InitializeRenderer(Color::black, Vector2Int{ window.getWidth(), window.getHeigth() }, default_cam_entity->getComponentOfClass<CameraComponent>());
	std::cout << " Done.\n";


	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << std::endl << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	// Initialize input system
	std::cout << "Initializing inputs...";
	Input::Initialize();
	std::cout << " Done.\n";

	// Create physics manager
	std::cout << "Initializing physics...";
	Locator::providePhysics(std::make_shared<PhysicsManager>());
	std::cout << " Done.\n";

	// Create audio manager
	std::cout << "Initializing audio...";
	Audio& audio = Locator::provideAudio(std::make_shared<AudioManager>());
	audio.Initialize(100.0f);
	std::cout << " Done.\n";


	// Initialize asset manager
	std::cout << "Initializing asset manager...";
	AssetManager::LoadNullAssets();
	EngineAssets::LoadEngineAssets();
	std::cout << " Done.\n";

	// Initialize debug manager
	std::cout << "Initializing debug manager...";
	DebugManager::InitializeDebugManager(*this);
	std::cout << " Done.\n";


	// Set global OpenGL properties
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	std::cout << "\nEngine initialization: " << glfwGetTime() << " seconds.\n";

	std::cout << "\nCy-Engine is ready to run.\n\n\n";


	return true;
}


void Engine::run()
{
	lastFrame = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		// Time logic part
		// -----------------
		double current_frame = glfwGetTime();
		deltaTime = static_cast<float>(current_frame - lastFrame);
		lastFrame = current_frame;

		GameplayStatics::SetDeltaTime(deltaTime);


		// Inputs update part
		// --------------------
		Input::UpdateInputSystem(); // Update the keys that were registered during the last frame


		// Engine / game update part
		// -------------
		engineUpdate(window.getGLFWwindow());

		const bool game_paused = DebugManager::GetPauseState();
		const bool one_frame = DebugManager::ConsumeOneFrameAdvance();
		if (!game_paused || (game_paused && one_frame))
		{
			if (game) game->update(deltaTime);

			GameplayComponentsManager::Update(deltaTime);
			ECS::Update(deltaTime);

			Locator::getPhysics().UpdatePhysics(deltaTime);
		}


		// Rendering part
		// ----------------
		Renderer& renderer = Locator::getRenderer();
		renderer.Draw();


		// Audio part
		// ------------
		Audio& audio = Locator::getAudio();
		const CameraComponent& active_cam = renderer.GetCamera();
		audio.UpdateListener(active_cam.getCamPosition(), active_cam.getCamUp(), active_cam.getCamForward());
		audio.Update();

		
		// Log part
		// ----------
		Locator::getLog().UpdateScreenLogs(deltaTime);


		// Debug part
		// ------------
		DebugManager::UpdateDebugManager(deltaTime);


		// Late update part
		// ------------
		if (game) game->lateUpdate();
		ECS::DeletePendings();


		// Time logic end of frame part
		// ------------------------
		const float engine_time = static_cast<float>(glfwGetTime() - current_frame);
		GameplayStatics::SetEngineTime(engine_time);


		// Events and buffer swap part
		// -----------------------------
		glfwSwapBuffers(window.getGLFWwindow());
		glfwPollEvents();
	}

	// Close engine (loop exit)
	unloadGame();
	clearEntities();
	ECS::Clear(true);
	AssetManager::ClearAllAssets();
	EngineAssets::UnloadEngineAssets();
	Locator::getAudio().Quit();
	Locator::initialize(); // Reset locator to null services (delete the real services)
}


void Engine::close()
{
	// Properly clear GLFW before closing app
	glfwTerminate();
}

void Engine::loadGame(std::weak_ptr<Game> game_)
{
	game = game_.lock();
	GameplayStatics::SetCurrentGame(game.get());
	game->load();
}

void Engine::unloadGame()
{
	if (game) game->unload();
	GameplayStatics::SetCurrentGame(nullptr);
}


void Engine::engineUpdate(GLFWwindow* glWindow)
{
	// Close window when escape is pressed
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(glWindow, true);
	}
}


// Window resize callback functions
void Engine::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); // Resize OpenGL viewport when GLFW is resized
	window.changeSize(width, height);

	Locator::getLog().LogMessage_Category("Window: Size updated to [Width: " + std::to_string(width) + " | Height: " + std::to_string(height) + "]", LogCategory::Info);

	Vector2Int window_size(width, height);
	Locator::getRenderer().SetWindowSize(window_size);
	GameplayStatics::SetWindowSize(window_size);
	GameplayStatics::OnScrenResize.broadcast(window_size);
}
