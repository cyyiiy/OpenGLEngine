#pragma once
#include <Core/game.h>
#include <Scenes/benchmarkRendering3D.h>
#include <Scenes/benchmarkEnd.h>


class BenchmarkGame : public Game
{
	enum BenchmarkState
	{
		Null,
		Rendering3D,
		Rendering2D,
		Physics,
		UpdateLoop,
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
	BenchmarkEnd benchmarkEnd;

	// Benchmark variables
	BenchmarkState currentBenchmarkState{ BenchmarkState::Null };
	bool currentStateFirstFrame{ false };
	float currentStateTime{ 0.0f };
	int currentStateFrameNumber{ 0 };
	float currentStateMinFrameTime{ 0.0f };
	float currentStateMaxFrameTime{ 0.0f };
};

