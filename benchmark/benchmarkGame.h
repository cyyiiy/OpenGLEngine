#pragma once
#include <Core/game.h>
#include <Scenes/benchmarkRendering3D.h>
#include <Scenes/benchmarkRendering2D.h>
#include <Scenes/benchmarkPhysics.h>
#include <Scenes/benchmarkECS.h>
#include <Scenes/benchmarkEnd.h>


class BenchmarkGame : public Game
{
	enum BenchmarkState
	{
		Null,
		Rendering3D,
		Rendering2D,
		Physics,
		ECS
	};

protected:
	void loadGameAssets() override;
	void loadGame() override;
	void unloadGame() override;
	void updateGame(float dt) override;

private:
	void loadProp(const std::string& name);
	void unloadProp(const std::string& name);

	void startBenchmarkState(BenchmarkState state);

	// Scenes
	BenchmarkRendering3D benchmarkRendering3D;
	BenchmarkRendering2D benchmarkRendering2D;
	BenchmarkPhysics benchmarkPhysics;
	BenchmarkECS benchmarkECS;
	BenchmarkEnd benchmarkEnd;

	// Benchmark variables
	BenchmarkState currentBenchmarkState{ BenchmarkState::Null };
	bool currentStateFirstFrame{ false };
	float currentStateTime{ 0.0f };
	std::vector<float> currentStateFrames;
};

