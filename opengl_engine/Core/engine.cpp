#include "engine.h"
#include <Assets/assetManager.h>
#include <Assets/defaultAssets.h>
#include <Assets/assetsIDs.h>
#include <Inputs/input.h>
#include <ServiceLocator/locator.h>
#include <Physics/physicsManager.h>
#include <GameplayStatics/gameplayStatics.h>
#include <ECS/componentManager.h>
#include <ECS/EngineComponents/engineComponents.h>
#include <iostream>

#include <chrono>


Engine::Engine()
{
}


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


	//  create window and initialize glfw
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
	); //  link window resize callback function

	glfwSetCursorPosCallback(gl_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			Input::ProcessMouse(window, xpos, ypos);
		}
	); //  link mouse pos callback function

	glfwSetScrollCallback(gl_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Input::ProcessScroll(window, xoffset, yoffset);
		}
	); //  link mouse scroll callback function

	glfwSetKeyCallback(gl_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Input::ProcessKeyboard(window, key, scancode, action, mods);
		}
	); //  link keyboard callback function

	glfwSetMouseButtonCallback(gl_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Input::ProcessMouseButton(window, button, action, mods);
		}
	); //  link mouse button callback function


	//  initialize service locator
	std::cout << "Initializing service locator...";
	Locator::initialize();
	std::cout << " Done.\n";


	//  create log manager
	std::cout << "Initializing log...";
	log = new LogManager(createEntity());
	Locator::provideLog(log);
	log->initialize();
	std::cout << " Done.\n";


	//  register engine components
	EngineComponents::RegisterEngineComponents();


	//  create renderer and default camera
	std::cout << "Initializing renderer...";
	Entity* default_cam_entity = createEntity();
	default_cam_entity->addComponentByClass<CameraComponent>();
	renderer = new RendererOpenGL();
	Locator::provideRenderer(renderer);
	renderer->initializeRenderer(Color::black, Vector2Int{ window.getWidth(), window.getHeigth() }, default_cam_entity->getComponentByClass<CameraComponent>());
	std::cout << " Done.\n";


	//  initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << std::endl << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	//  initialize input system
	std::cout << "Initializing inputs...";
	Input::Initialize();
	std::cout << " Done.\n";

	//  initialize physics
	std::cout << "Initializing physics...";
	Physics& physics = Locator::providePhysics(new PhysicsManager());
	physics.InitialisePhysics();
	std::cout << " Done.\n";

	//  initialize audio manager
	std::cout << "Initializing audio...";
	audio = new AudioManager();
	Locator::provideAudio(audio);
	audio->Initialize(100.0f);
	std::cout << " Done.\n";


	//  load "null" assets of AssetManager
	std::cout << "Initializing asset manager...";
	AssetManager::LoadNullAssets();

	//  initialize assets IDs
	AssetsIDs::InitializeAssetIDs();

	//  load engine assets
	DefaultAssets::LoadEngineAssets();
	std::cout << " Done.\n";


	//  intialize debug fps text
	fpsText = createEntity()->addComponentByClass<TextRendererComponent>();
	fpsText->setTextDatas("FPS: 0", AssetManager::GetFont("arial_64"), Vector2::one, Vector2::one, Vector2{ -20.0f, -20.0f }, Vector2{ 0.5f }, 0.0f, Color::white);
	fpsText->setEnabled(false);


	//  initialize debug camera
	debugCamEntity = createEntity();
	debugCamera = debugCamEntity->addComponentByClass<CameraComponent>();
	renderer->setDebugCamera(debugCamera);


	//  configure global OpenGL properties
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

	//  main loop
	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		//  time logic part
		// -----------------
		double current_frame = glfwGetTime();
		deltaTime = static_cast<float>(current_frame - lastFrame);
		lastFrame = current_frame;

		GameplayStatics::SetDeltaTime(deltaTime);


		//  inputs update part
		// --------------------
		Input::UpdateInputSystem(); //  update the keys that were registered during the last frame


		//  update part
		// -------------
		engineUpdate(window.getGLFWwindow());

		if (!gamePaused || (gamePaused && oneFrame))
		{
			if (game) game->update(deltaTime);

			ComponentManager::UpdateComponents(deltaTime);

			Locator::getPhysics().UpdatePhysics(deltaTime);

			oneFrame = false;
		}


		//  rendering part
		// ----------------
		renderer->draw();
		renderer->updateDebugDraws(gamePaused ? 0.0f : deltaTime); //  debug draws don't expire if engine is paused



		//  audio part
		// ------------
		const CameraComponent& active_cam = *renderer->GetCamera();
		audio->UpdateListener(active_cam.getCamPosition(), active_cam.getCamUp(), active_cam.getCamForward());
		audio->Update();

		
		//  log part
		// ----------
		log->updateScreenLogs(deltaTime);


		if (game) game->lateUpdate();


		//  time logic second part
		// ------------------------
		const float engine_time = static_cast<float>(glfwGetTime() - current_frame);
		GameplayStatics::SetEngineTime(engine_time);


		//  events and buffer swap part
		// -----------------------------
		glfwSwapBuffers(window.getGLFWwindow());
		glfwPollEvents();
	}

	//  close engine
	unloadGame();
	audio->Quit();
	clearEntities();
	delete log;
}


void Engine::close()
{
	//  properly clear GLFW before closing app
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
	//  close window when escape is pressed
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(glWindow, true);
	}

	//  pause and freecam are useless if there is no active game or scene
	if (!game || !game->hasActiveScene()) return;

	//  pause/unpause the game when p is pressed
	if (Input::IsKeyPressed(GLFW_KEY_P))
	{
		if (!gamePaused) pauseGame();
		else unpauseGame();
	}

	//  make the engine run only one frame when o is pressed
	if (Input::IsKeyPressed(GLFW_KEY_O))
	{
		advanceOneFrame();
	}

	//  active/desactive the freecam mode when m is pressed
	if (Input::IsKeyPressed(GLFW_KEY_SEMICOLON))
	{
		if (!freecamMode) enableFreecam();
		else disableFreecam();
	}

	//  active/desactive the debug view mode when k is pressed
	if (Input::IsKeyPressed(GLFW_KEY_K))
	{
		if (!debugViewMode) enableDebugView();
		else disableDebugView();
	}



	if (freecamMode)
	{
		//  move freecam
		if (Input::IsKeyDown(GLFW_KEY_W))
			debugCamEntity->addPosition(debugCamera->getCamForward() * debugCameraSpeed * deltaTime);

		if (Input::IsKeyDown(GLFW_KEY_S))
			debugCamEntity->addPosition(-debugCamera->getCamForward() * debugCameraSpeed * deltaTime);

		if (Input::IsKeyDown(GLFW_KEY_A))
			debugCamEntity->addPosition(debugCamera->getCamRight() * debugCameraSpeed * deltaTime);

		if (Input::IsKeyDown(GLFW_KEY_D))
			debugCamEntity->addPosition(-debugCamera->getCamRight() * debugCameraSpeed * deltaTime);

		if (Input::IsKeyDown(GLFW_KEY_SPACE))
			debugCamEntity->addPosition(Vector3::unitY * debugCameraSpeed * deltaTime);

		if (Input::IsKeyDown(GLFW_KEY_C))
			debugCamEntity->addPosition(Vector3::negUnitY * debugCameraSpeed * deltaTime);

		if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			debugCameraSpeed = 10.0f;

		if (Input::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
			debugCameraSpeed = 4.0f;

		Vector2 mouse_delta = Input::GetMouseDelta() * debugCameraMouseSensitivity;
		debugCamera->addYaw(-mouse_delta.x);
		debugCamera->setPitch(Maths::clamp(debugCamera->getPitch() + mouse_delta.y, -89.0f, 89.0f));

		float scroll_offset = Input::GetScrollOffset();
		debugCamera->setFov(Maths::clamp(debugCamera->getFov() - scroll_offset, 1.0f, 45.0f));
	}


	if (debugViewMode)
	{
		//  update fps counter
		frameCounter++;
		frameTimeCounter += deltaTime;
		if (frameTimeCounter >= 1.0f)
		{
			frameTimeCounter -= 1.0f;
			fpsText->setText("FPS: " + std::to_string(frameCounter));
			frameCounter = 0;
		}
	}
}


void Engine::pauseGame()
{
	gamePaused = true;
	audio->PauseAll();
	log->LogMessage_Category("Game paused", LogCategory::Info);
}

void Engine::unpauseGame()
{
	gamePaused = false;
	audio->ResumeAll();
	if (freecamMode) disableFreecam();
	log->LogMessage_Category("Game unpaused", LogCategory::Info);
}

void Engine::advanceOneFrame()
{
	if (!gamePaused) pauseGame();
	else
	{
		log->LogMessage_Category("Debug: Advance one frame", LogCategory::Info);
		oneFrame = true;
	}
}

void Engine::enableFreecam()
{
	freecamMode = true;
	if (!gamePaused) pauseGame();
	log->LogMessage_Category("Debug: Freecam mode enabled", LogCategory::Info);
	debugCamera->copyCamera(*renderer->GetCamera(), true);
	renderer->setDebugCamActivated(true);
	debugCameraSpeed = 4.0f;
}

void Engine::disableFreecam()
{
	freecamMode = false;
	log->LogMessage_Category("Debug: Freecam mode disabled", LogCategory::Info);
	renderer->setDebugCamActivated(false);
}

void Engine::enableDebugView()
{
	debugViewMode = true;
	log->LogMessage_Category("Debug: Debug mode view enabled", LogCategory::Info);
	renderer->physicsDebugMode = true;
	fpsText->setEnabled(true);
}

void Engine::disableDebugView()
{
	debugViewMode = false;
	log->LogMessage_Category("Debug: Debug mode view disabled", LogCategory::Info);
	renderer->physicsDebugMode = false;
	fpsText->setEnabled(false);
}


//  window resize callback functions
void Engine::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); //  resize OpenGL viewport when GLFW is resized
	window.changeSize(width, height);

	log->LogMessage_Category("Window: Size updated to [Width: " + std::to_string(width) + " | Height: " + std::to_string(height) + "]", LogCategory::Info);

	Vector2Int window_size(width, height);
	renderer->setWindowSize(window_size);
	GameplayStatics::SetWindowSize(window_size);
	GameplayStatics::OnScrenResize.broadcast(window_size);
}
