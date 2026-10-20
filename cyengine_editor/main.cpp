#include "EditorCore/editor.h"


int main()
{
	Editor editor;
	bool is_editor_init = editor.initialize(1920, 1080);

	if (!is_editor_init) return -1;

	editor.run();
	editor.close();

	return 0;
}