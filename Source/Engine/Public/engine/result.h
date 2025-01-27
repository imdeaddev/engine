#pragma once
#include <cstdint>

namespace engine {
enum class EResult : int32_t {
    UnknownError = -1,
    Success = 0,
    APIError = -10000,
    FilesystemError,
};

constexpr bool succeeded(EResult result) { return static_cast<int32_t>(result) >= 0; }
constexpr bool failed(EResult result) { return static_cast<int32_t>(result) < 0; }
} // namespace engine
