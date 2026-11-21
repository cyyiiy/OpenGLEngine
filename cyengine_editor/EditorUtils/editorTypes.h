#pragma once
#include <string>

namespace EditorTypes
{
	struct NewProjectData
	{
		NewProjectData() {}

		bool windowOpened{ false };
		std::string projectName{ "" };
		std::string folderPath{ "" };
	};
}