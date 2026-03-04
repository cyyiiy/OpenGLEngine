#include <Core/engine.h>
#include "benchmarkGame.h"


int main()
{
	Engine engine;
	bool is_engine_init = engine.initialize();

	if (!is_engine_init) return -1;

	engine.loadGame(std::make_shared<BenchmarkGame>());

	engine.run();
	engine.close();

	return 0;
}