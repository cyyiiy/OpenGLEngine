#include "editor.h"
#include <ServiceLocator/locator.h>
#include <Rendering/rendererOpenGL.h>
#include <Assets/assetManager.h>
#include <Assets/engineAssets.h>
#include <EditorUtils/editorAssets.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <EditorGUI/Windows/editorWindows.h>

#include <iostream>


bool Editor::initialize(int wndw_width, int wndw_height)
{
	std::cout << "Initializing Cy-Engine editor...\n";

	// Create and initialize the window
	window.createWindow(wndw_width, wndw_height, "Cy-Engine Editor", false);
	GLFWwindow* gl_window = window.getGLFWwindow();
	if (gl_window == nullptr)
	{
		std::cout << "Failed to create GLFW window.\n";
		glfwTerminate();
		return false;
	}

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(gl_window, true);
	ImGui_ImplOpenGL3_Init();

	// Link the GLFW callbacks
	glfwSetWindowUserPointer(gl_window, this);

	glfwSetFramebufferSizeCallback(gl_window, [](GLFWwindow* window, int width, int height)
		{
			auto self = static_cast<Editor*>(glfwGetWindowUserPointer(window));
			self->windowResize(window, width, height);
		}
	); // Link window resize callback function

	// Initialize the service locator
	Locator::initialize();

	// Create renderer and default camera
	Entity* default_cam_entity = createEntity();
	default_cam_entity->addComponentByClass<CameraComponent>();
	Renderer& renderer = Locator::provideRenderer(std::make_shared<RendererOpenGL>());
	renderer.InitializeRenderer(Color::black, Vector2Int{ window.getWidth(), window.getHeigth() }, default_cam_entity->getComponentOfClass<CameraComponent>());

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD.\n";
		close();
		return false;
	}

	// Initialize asset manager
	EngineAssets::LoadNullAssets();
	EngineAssets::LoadEngineAssets();
	EditorAssets::LoadEditorAssets();

	// Set global OpenGL properties
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "Editor initialization successful!\n";

	return true;
}

void Editor::run()
{
	lastFrame = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		// Poll OpenGL events
		glfwPollEvents();

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// TODO: Move this in a function that create the entire editor GUI
		EditorWindows::ShowViewportWindow();

		// Time logic
		double current_frame = glfwGetTime();
		deltaTime = static_cast<float>(current_frame - lastFrame);
		lastFrame = current_frame;

		// Render the engine
		Renderer& renderer = Locator::getRenderer();
		renderer.Draw();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(window.getGLFWwindow());
	}

	// Close editor
	clearEntities();
	ECS::Clear(true);
	AssetManager::ClearAllAssets();
	EditorAssets::UnloadEditorAssets();
	EngineAssets::UnloadEngineAssets();
	Locator::initialize(); // Reset locator to null services (delete the real services)
}

void Editor::close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Editor::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); // Resize OpenGL viewport when GLFW is resized
	window.changeSize(width, height);

	Vector2Int window_size(width, height);
	Locator::getRenderer().SetWindowSize(window_size);
}