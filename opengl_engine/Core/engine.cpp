#include "engine.h"
#include <Assets/assetManager.h>
#include <Assets/defaultAssets.h>
#include <Assets/assetsIDs.h>

#include <GameplayStatics/gameplayStatics.h>
#include <Inputs/input.h>
#include <ECS/ecs.h>
#include <Core/Debug/debugManager.h>

#include <ServiceLocator/locator.h>
#include <Rendering/rendererOpenGL.h>
#include <PhysicsAABB/physicsManager.h>
#include <Audio/audioManager.h>
#include <Log/logManager.h>

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
	Log& log = Locator::provideLog(std::make_shared<LogManager>());
	log.Initialize();
	std::cout << " Done.\n";


	//  create renderer and default camera
	std::cout << "Initializing renderer...";
	Entity* default_cam_entity = createEntity();
	default_cam_entity->addComponentByClass<CameraComponent>();
	Renderer& renderer = Locator::provideRenderer(std::make_shared<RendererOpenGL>());
	renderer.InitializeRenderer(Color::black, Vector2Int{ window.getWidth(), window.getHeigth() }, default_cam_entity->getComponentOfClass<CameraComponent>());
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
	Locator::providePhysics(std::make_shared<PhysicsManager>());
	std::cout << " Done.\n";

	//  initialize audio manager
	std::cout << "Initializing audio...";
	Audio& audio = Locator::provideAudio(std::make_shared<AudioManager>());
	audio.Initialize(100.0f);
	std::cout << " Done.\n";


	//  load "null" assets of AssetManager
	std::cout << "Initializing asset manager...";
	AssetManager::LoadNullAssets();

	//  initialize assets IDs
	AssetsIDs::InitializeAssetIDs();

	//  load engine assets
	DefaultAssets::LoadEngineAssets();
	std::cout << " Done.\n";

	//  initialize debug manager
	std::cout << "Initializing debug manager...";
	DebugManager::InitializeDebugManager(*this);
	std::cout << " Done.\n";


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

		const bool game_paused = DebugManager::GetPauseState();
		const bool one_frame = DebugManager::ConsumeOneFrameAdvance();
		if (!game_paused || (game_paused && one_frame))
		{
			if (game) game->update(deltaTime);

			ECS::Update(deltaTime);
			GameplayStatics::UpdateDebugs(game_paused ? 0.0f : deltaTime); //  debug draws don't expire if engine is paused

			Locator::getPhysics().UpdatePhysics(deltaTime);
		}


		//  rendering part
		// ----------------
		Renderer& renderer = Locator::getRenderer();
		renderer.Draw();



		//  audio part
		// ------------
		Audio& audio = Locator::getAudio();
		const CameraComponent& active_cam = renderer.GetCamera();
		audio.UpdateListener(active_cam.getCamPosition(), active_cam.getCamUp(), active_cam.getCamForward());
		audio.Update();

		
		//  log part
		// ----------
		Locator::getLog().UpdateScreenLogs(deltaTime);


		DebugManager::UpdateDebugManager(deltaTime);

		if (game) game->lateUpdate();
		ECS::DeletePendings();


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
	clearEntities();
	ECS::Clear(true);
	AssetManager::ClearAllAssets();
	Locator::getAudio().Quit();
	Locator::initialize(); //  reset locator to null services (delete the real services)
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
}


//  window resize callback functions
void Engine::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); //  resize OpenGL viewport when GLFW is resized
	window.changeSize(width, height);

	Locator::getLog().LogMessage_Category("Window: Size updated to [Width: " + std::to_string(width) + " | Height: " + std::to_string(height) + "]", LogCategory::Info);

	Vector2Int window_size(width, height);
	Locator::getRenderer().SetWindowSize(window_size);
	GameplayStatics::SetWindowSize(window_size);
	GameplayStatics::OnScrenResize.broadcast(window_size);
}
