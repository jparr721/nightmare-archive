#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace nm::testing::scenes {
    inline const char *pathToPathsFile = __FILE__;

    // Path to the assets folder
    inline const fs::path kAssetsPath = fs::path(pathToPathsFile).parent_path().parent_path() / fs::path("assets");
}// namespace nm::testing::scenes