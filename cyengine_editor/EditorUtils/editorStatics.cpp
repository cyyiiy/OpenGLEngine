#include "editorStatics.h"
#include <Core/window.h>

bool EditorStatics::isViewportFocused = false;
std::vector<LogMessage> EditorStatics::logMessages{};
Window* EditorStatics::window = nullptr;