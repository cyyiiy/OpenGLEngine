#include <Core/engine.h>


int main()
{
	Engine engine;
	bool is_engine_init = engine.initialize(1920, 1080, "CyEngine Editor", false);

	if (!is_engine_init) return -1;

	engine.run();
	engine.close();

	return 0;
}