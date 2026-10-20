#include "EditorCore/editor.h"

#include <Assets/defaultAssets.h>
#include <EditorCore/scenesManager.h>
#include <Scenes/demoScene.h>


int main()
{
	Editor editor;
	bool is_editor_init = editor.initialize(1920, 1080);

	if (!is_editor_init) return -1;

	// Note: Temporary for testing
	DefaultAssets::LoadDefaultAssets();
	DemoScene demo_scene;
	ScenesManager::loadScene(&demo_scene);

	editor.run();
	editor.close();

	return 0;
}