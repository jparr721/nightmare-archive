#include "nm_io.h"

#include <filesystem>

namespace nm {
    auto getFileExtension(const std::string& file) -> std::string {
        return std::filesystem::path(file).extension();
    }
}