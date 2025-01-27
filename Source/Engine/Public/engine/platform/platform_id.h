#pragma once

namespace engine {
enum class EPlatformId { Windows, Linux };

#if defined(_WIN32)
constexpr EPlatformId currentPlatform = EPlatformId::Windows;
#elif defined(__linux__)
constexpr EPlatformId currentPlatform = EPlatformId::Linux;
#else
static_assert(false, "Current platform is undefined");
#endif
} // namespace engine
