#include "guiHelper.h"

std::string GuiHelper::CombineChars(const char* a, const char* b)
{
	return std::string(a) + std::string(b);
}

std::string GuiHelper::WindowLayout(const char* window, const char* layout)
{
	return std::string(window) + "##Layout_" + std::string(layout);
}
