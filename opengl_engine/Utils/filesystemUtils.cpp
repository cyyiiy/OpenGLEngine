#include "filesystemUtils.h"

std::vector<std::filesystem::path> FilesystemUtils::ConvertStringsToPaths(const std::vector<std::string>& stringPaths)
{
    std::vector<std::filesystem::path> out_paths;
    out_paths.reserve(stringPaths.size());

    for (const std::string& string_path : stringPaths)
    {
        out_paths.push_back(string_path);
    }

    return out_paths;
}
