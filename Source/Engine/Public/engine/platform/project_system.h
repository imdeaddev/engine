#pragma once
#include <engine/export_macros.h>
#include <string_view>
#include <string>
#include <cstdint>

namespace engine {
struct Version {
    uint32_t major = 0, minor = 0, patch = 0;
    constexpr Version(uint32_t major_ = 0, uint32_t minor_ = 0, uint32_t patch_ = 0) : major(major_), minor(minor_), patch(patch_) {}
    constexpr Version &set_major(uint32_t v) {
        major = v;
        return *this;
    }
    constexpr Version &set_minor(uint32_t v) {
        minor = v;
        return *this;
    }
    constexpr Version &set_patch(uint32_t v) {
        patch = v;
        return *this;
    }
};

class ENGINEAPI ProjectSystem {
    ProjectSystem() = default;
    ProjectSystem(const ProjectSystem &) = delete;
    ProjectSystem &operator=(const ProjectSystem &) = delete;
    std::string m_projectName{};
    Version m_projectVersion;

public:
    static ProjectSystem &instance();

    void set_project_info(const std::string_view projectName, Version projectVersion);

    constexpr const std::string &get_project_name() const { return m_projectName; }
    constexpr const Version &get_project_version() const { return m_projectVersion; }
};

inline auto &project_system() { return ProjectSystem::instance(); }
} // namespace engine
