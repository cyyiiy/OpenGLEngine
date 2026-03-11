#include "benchmarkGame.h"

#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Assets/defaultAssets.h>

#include <GLFW/glfw3.h>
#include <sstream>
#include <numeric>


void BenchmarkGame::loadGameAssets()
{
	Log& log = Locator::getLog();
	log.SetConsoleLogDisplayRule(LogCategory::Info);

	log.LogMessage_Category("Benchmark: Start loading assets...", LogCategory::Info);
	const double load_start_time = glfwGetTime();
	double load_time = load_start_time;

	// Load default assets
	DefaultAssets::LoadDefaultAssets();
	AssetManager::CreateShaderProgram("lit_object", "Lit/object_lit.vert", "Lit/object_lit.frag", ShaderType::Lit);
	log.LogMessage_Category("Benchmark: Load default assets time: " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();

	// Load benchmark floor
	AssetManager::LoadTexture("floor_diffuse", "benchmark/textures/stonefloor/stonefloor_basecolor.jpg", false);
	AssetManager::LoadTexture("floor_specular", "benchmark/textures/stonefloor/stonefloor_specular.jpg", false);
	Material& floor_mat = AssetManager::CreateMaterial("floor", AssetManager::GetShader("lit_object"));
	floor_mat.addTexture(&AssetManager::GetTexture("floor_diffuse"), TextureType::Diffuse);
	floor_mat.addTexture(&AssetManager::GetTexture("floor_specular"), TextureType::Specular);
	floor_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	floor_mat.addParameter("material.shininess", 32.0f);
	floor_mat.addParameter("beta_prevent_tex_scaling", true);
	floor_mat.addParameter("beta_tex_scaling_factor", 2.0f);
	AssetManager::CreateModel("floor").
		addMesh(AssetManager::GetSingleMesh("default_plane"), AssetManager::GetMaterial("floor"));
	log.LogMessage_Category("Benchmark: Loaded floor texture in " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();

	// Load benchmark sprites
	AssetManager::LoadTexture("sprite_matrix", "benchmark/sprites/matrix.jpg", false);

	// Load benchmark props
	loadProp("woodenchest");
	loadProp("romanstatue");
	loadProp("orangebrick");
	log.LogMessage_Category("Benchmark: Load props time: " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();


	log.LogMessage_Category("Benchmark: Finished loading assets in " + std::to_string(glfwGetTime() - load_start_time) + " seconds.", LogCategory::Info);
}

void BenchmarkGame::loadGame()
{
	startBenchmarkState(BenchmarkState::Rendering3D);
}

void BenchmarkGame::unloadGame()
{
	AssetManager::DeleteModel("floor");
	AssetManager::DeleteMaterial("floor");
	AssetManager::DeleteTexture("floor_diffuse");
	AssetManager::DeleteTexture("floor_specular");

	AssetManager::DeleteTexture("sprite_matrix");

	unloadProp("woodenchest");
	unloadProp("romanstatue");
	unloadProp("orangebrick");
}

void BenchmarkGame::updateGame(float dt)
{
	if (currentBenchmarkState == BenchmarkState::Null) return;

	if (!currentStateFirstFrame)
	{
		currentStateFirstFrame = true;
		return;
	}

	currentStateTime += dt;
	currentStateFrames.push_back(dt);

	if (currentStateTime < 5.0f) return;

	// Compute benchmark results
	std::vector<float> shortest_fives = currentStateFrames, longest_fives = currentStateFrames;
	std::partial_sort(shortest_fives.begin(), shortest_fives.begin() + 5, shortest_fives.end());
	std::partial_sort(longest_fives.begin(), longest_fives.begin() + 5, longest_fives.end(), std::greater<>());
	shortest_fives.resize(5);
	longest_fives.resize(5);

	float average = std::accumulate(currentStateFrames.begin(), currentStateFrames.end(), 0.0f);
	average /= currentStateFrames.size();

	// Log benchmark results
	Log& log = Locator::getLog();

	log.LogMessage_Category("Benchmark: Top 5 shortest frame times:", LogCategory::Info);
	for (int i = 0; i < 5; i++)
	{
		std::stringstream msg;
		msg << "#" << i + 1 << ": " << shortest_fives[i] * 1000.0f << " ms (" << Maths::round(1.0f / shortest_fives[i]) << " FPS).";
		log.LogMessage_Category(msg.str(), LogCategory::Info);
	}

	log.LogMessage_Category("Benchmark: Top 5 longest frame times:", LogCategory::Info);
	for (int i = 0; i < 5; i++)
	{
		std::stringstream msg;
		msg << "#" << i + 1 << ": " << longest_fives[i] * 1000.0f << " ms (" << Maths::round(1.0f / longest_fives[i]) << " FPS).";
		log.LogMessage_Category(msg.str(), LogCategory::Info);
	}

	std::stringstream msg;
	msg << "Benchmark: Average frame time: " << average * 1000.0f << " ms (" << Maths::round(1.0f / average) << " FPS).";
	log.LogMessage_Category(msg.str(), LogCategory::Info);

	// Start next benchmark state
	switch (currentBenchmarkState)
	{
	case Rendering3D:
		log.LogMessage_Category("Benchmark: =============== End 3D Rendering benchmark =================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Rendering2D);
		break;

	case Rendering2D:
		log.LogMessage_Category("Benchmark: =============== End 2D Rendering benchmark =================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Physics);
		break;

	case Physics:
		log.LogMessage_Category("Benchmark: =============== End Physics benchmark ======================", LogCategory::Info);
		loadScene(&benchmarkEnd);
		currentBenchmarkState = BenchmarkState::Null;
		break;

	default:
		log.LogMessage_Category("Benchmark: Tried to analyze unimplemented benchmark state.", LogCategory::Error);
		return;
	}
}

void BenchmarkGame::loadProp(const std::string& name)
{
	const double load_prop_time = glfwGetTime();

	const std::string prop_path = "benchmark/props/" + name + "/" + name;

	// Load prop textures
	AssetManager::LoadTexture(name + "_diffuse", prop_path + "_basecolor.jpg");
	AssetManager::LoadTexture(name + "_specular", prop_path + "_specular.jpg");

	// Create prop material
	Material& prop_mat = AssetManager::CreateMaterial(name, AssetManager::GetShader("lit_object"));
	prop_mat.addTexture(&AssetManager::GetTexture(name + "_diffuse"), TextureType::Diffuse);
	prop_mat.addTexture(&AssetManager::GetTexture(name + "_specular"), TextureType::Specular);
	prop_mat.addTexture(&AssetManager::GetTexture("default_black"), TextureType::Emissive);
	prop_mat.addParameter("material.shininess", 32.0f);

	// Load prop mesh and create model
	AssetManager::LoadMeshCollection(name, prop_path + ".fbx");
	AssetManager::CreateModel(name).
		addMeshes(AssetManager::GetMeshCollection(name), AssetManager::GetMaterial(name));

	Locator::getLog().LogMessage_Category(
		"Benchmark: Loaded prop \"" + name + "\" in " + std::to_string(glfwGetTime() - load_prop_time) + " seconds.", 
		LogCategory::Info);
}

void BenchmarkGame::unloadProp(const std::string& name)
{
	AssetManager::DeleteModel(name);
	AssetManager::DeleteMeshCollection(name);
	AssetManager::DeleteMaterial(name);
	AssetManager::DeleteTexture(name + "_diffuse");
	AssetManager::DeleteTexture(name + "_specular");
}

void BenchmarkGame::startBenchmarkState(BenchmarkState state)
{
	Log& log = Locator::getLog();
	const double load_scene_time = glfwGetTime();

	switch (state)
	{
	case Rendering3D:
		log.LogMessage_Category("Benchmark: =============== Start 3D Rendering benchmark ===============", LogCategory::Info);
		loadScene(&benchmarkRendering3D);
		log.LogMessage_Category("Benchmark: Loaded 3D Rendering scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
		break;

	case Rendering2D:
		log.LogMessage_Category("Benchmark: =============== Start 2D Rendering benchmark ===============", LogCategory::Info);
		loadScene(&benchmarkRendering2D);
		log.LogMessage_Category("Benchmark: Loaded 2D Rendering scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
		break;

	case Physics:
		log.LogMessage_Category("Benchmark: =============== Start Physics benchmark ====================", LogCategory::Info);
		loadScene(&benchmarkPhysics);
		log.LogMessage_Category("Benchmark: Loaded Physics scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
		break;

	default:
		log.LogMessage_Category("Benchmark: Tried to start unimplemented benchmark state.", LogCategory::Error);
		return;
	}

	log.LogMessage_Category("Benchmark: Analyzing performances... (wait 5 seconds)", LogCategory::Info);
	currentStateFirstFrame = false;
	currentStateTime = 0.0f;
	currentStateFrames.clear();
	currentBenchmarkState = state;
}
