#include "benchmarkGame.h"

#include <ServiceLocator/locator.h>
#include <Assets/assetManager.h>
#include <Assets/defaultAssets.h>
#include <GameplayStatics/gameplayStatics.h>

#include <Rendering/texture.h>
#include <Rendering/material.h>
#include <Rendering/Model/model.h>
#include <Rendering/Text/font.h>

#include <GLFW/glfw3.h>
#include <sstream>
#include <numeric>

const std::string BENCHMARK_GLOBAL_GROUP = "BenchmarkGlobalAssets";


void BenchmarkGame::loadGameAssets()
{
	Log& log = Locator::getLog();
	log.SetConsoleLogDisplayRule(LogCategory::Info);

	log.LogMessage_Category("Benchmark: Start loading assets...", LogCategory::Info);
	const double load_start_time = glfwGetTime();
	double load_time = load_start_time;

	// Load default assets
	DefaultAssets::LoadDefaultAssets();
	log.LogMessage_Category("Benchmark: Load default assets time: " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();

	AssetManager::OpenLoadingGroup(BENCHMARK_GLOBAL_GROUP);

	// Load benchmark floor
	AssetManager::LoadAsset<Texture>("floor_diffuse", { "benchmark/textures/stonefloor/stonefloor_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>("floor_specular", { "benchmark/textures/stonefloor/stonefloor_specular.jpg", false });
	Material::LoadParams floor_mat(AssetManager::GetAsset<Shader>("lit_object"));
	floor_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>("floor_diffuse"));
	floor_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>("floor_specular"));
	floor_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	floor_mat.floatParameters.emplace("material.shininess", 32.0f);
	floor_mat.boolParameters.emplace("beta_prevent_tex_scaling", true);
	floor_mat.floatParameters.emplace("beta_tex_scaling_factor", 1.0f);
	AssetManager::LoadAsset<Material>("floor", floor_mat);
	log.LogMessage_Category("Benchmark: Loaded floor material in " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();

	// Load benchmark sprites
	AssetManager::LoadAsset<Texture>("sprite_matrix", { "benchmark/sprites/matrix.jpg", false });

	// Load benchmark fonts
	AssetManager::LoadAsset<Font>("arial_24", { "arial_font/arial.ttf", 24, CharacterLoading::ASCII_128 });

	// Load benchmark props
	loadProp("woodenchest");
	loadProp("romanstatue");
	loadProp("orangebrick");
	log.LogMessage_Category("Benchmark: Load props time: " + std::to_string(glfwGetTime() - load_time) + " seconds.", LogCategory::Info);
	load_time = glfwGetTime();

	AssetManager::CloseLoadingGroup();
	log.LogMessage_Category("Benchmark: Finished loading assets in " + std::to_string(glfwGetTime() - load_start_time) + " seconds.", LogCategory::Info);
}

void BenchmarkGame::loadGame()
{
	startBenchmarkState(BenchmarkState::Rendering3D);
}

void BenchmarkGame::unloadGame()
{
	AssetManager::TryUnloadAssetsOfGroup(BENCHMARK_GLOBAL_GROUP);
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
	currentStateEngineTimeSum += GameplayStatics::GetEngineTime();

	if (currentStateTime < 5.0f) return;

	// Compute benchmark results
	std::vector<float> shortest_fives = currentStateFrames, longest_fives = currentStateFrames;
	std::partial_sort(shortest_fives.begin(), shortest_fives.begin() + 5, shortest_fives.end());
	std::partial_sort(longest_fives.begin(), longest_fives.begin() + 5, longest_fives.end(), std::greater<>());
	shortest_fives.resize(5);
	longest_fives.resize(5);

	float average = std::accumulate(currentStateFrames.begin(), currentStateFrames.end(), 0.0f);
	average /= currentStateFrames.size();
	const float average_engine = currentStateEngineTimeSum / currentStateFrames.size();

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

	std::stringstream msg2;
	msg2 << "Benchmark: Average engine time: " << average_engine * 1000.0f << " ms (" << Maths::round(1.0f / average_engine) << " FPS).";
	log.LogMessage_Category(msg2.str(), LogCategory::Info);
	log.LogMessage_Category("Benchmark: Engine time is frame time without the OpenGL buffer swap.", LogCategory::Info);

	// Start next benchmark state
	switch (currentBenchmarkState)
	{
	case Rendering3D:
		log.LogMessage_Category("Benchmark: =============== End 3D Rendering benchmark =================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Materials);
		break;

	case Materials:
		log.LogMessage_Category("Benchmark: =============== End Materials benchmark ====================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Movement);
		break;

	case Movement:
		log.LogMessage_Category("Benchmark: =============== End Movement benchmark =====================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Rendering2D);
		break;

	case Rendering2D:
		log.LogMessage_Category("Benchmark: =============== End 2D Rendering benchmark =================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::Physics);
		break;

	case Physics:
		log.LogMessage_Category("Benchmark: =============== End Physics benchmark ======================", LogCategory::Info);
		startBenchmarkState(BenchmarkState::ECS);
		break;

	case ECS:
		log.LogMessage_Category("Benchmark: =============== End ECS benchmark ==========================", LogCategory::Info);
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
	AssetManager::LoadAsset<Texture>(name + "_diffuse", { prop_path + "_basecolor.jpg", false });
	AssetManager::LoadAsset<Texture>(name + "_specular", { prop_path + "_specular.jpg", false });

	// Create prop material
	Material::LoadParams prop_mat(AssetManager::GetAsset<Shader>("lit_object"));
	prop_mat.textures.emplace("diffuse", AssetManager::GetAsset<Texture>(name + "_diffuse"));
	prop_mat.textures.emplace("specular", AssetManager::GetAsset<Texture>(name + "_specular"));
	prop_mat.textures.emplace("emissive", AssetManager::GetAsset<Texture>("default_black"));
	prop_mat.floatParameters.emplace("material.shininess", 32.0f);
	AssetManager::LoadAsset<Material>(name, prop_mat);

	// Load prop model
	AssetManager::LoadAsset<Model>(name, Model::FileImportParams{ prop_path + ".fbx", { AssetManager::GetAsset<Material>(name) } });

	Locator::getLog().LogMessage_Category(
		"Benchmark: Loaded prop \"" + name + "\" in " + std::to_string(glfwGetTime() - load_prop_time) + " seconds.", 
		LogCategory::Info);
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

	case Materials:
		log.LogMessage_Category("Benchmark: =============== Start Materials benchmark ==================", LogCategory::Info);
		loadScene(&benchmarkMaterials);
		log.LogMessage_Category("Benchmark: Loaded Materials scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
		break;

	case Movement:
		log.LogMessage_Category("Benchmark: =============== Start Movement benchmark ===================", LogCategory::Info);
		loadScene(&benchmarkMovement);
		log.LogMessage_Category("Benchmark: Loaded Movement scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
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

	case ECS:
		log.LogMessage_Category("Benchmark: =============== Start ECS benchmark ========================", LogCategory::Info);
		loadScene(&benchmarkECS);
		log.LogMessage_Category("Benchmark: Loaded ECS scene in " + std::to_string((glfwGetTime() - load_scene_time) * 1000.0) + " ms.", LogCategory::Info);
		break;

	default:
		log.LogMessage_Category("Benchmark: Tried to start unimplemented benchmark state.", LogCategory::Error);
		return;
	} 

	log.LogMessage_Category("Benchmark: Analyzing performances... (wait 5 seconds)", LogCategory::Info);
	currentStateFirstFrame = false;
	currentStateTime = 0.0f;
	currentStateFrames.clear();
	currentStateEngineTimeSum = 0.0f;
	currentBenchmarkState = state;
}
