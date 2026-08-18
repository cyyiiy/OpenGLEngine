#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace FilesystemUtils
{
	/** Convert a vector of strings to a vector of filesystem paths. */
	[[nodiscard]] std::vector<std::filesystem::path> ConvertStringsToPaths(const std::vector<std::string>& stringPaths);
};

