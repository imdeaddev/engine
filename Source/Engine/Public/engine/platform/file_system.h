#pragma once
#include <filesystem>
#include <engine/export_macros.h>

namespace std {
namespace fs = std::filesystem;
}

namespace engine {
ENGINEAPI const std::fs::path &get_write_directory_path();
ENGINEAPI const std::fs::path &get_current_binary_folder();
}
