#include <engine/platform/project_system.h>

engine::ProjectSystem &engine::ProjectSystem::instance() {
    static ProjectSystem s_system{};
    return s_system;
}

void engine::ProjectSystem::set_project_info(const std::string_view projectName, Version projectVersion) {
    static bool s_setOnce = false;
    if (s_setOnce) {
        return;
    }
    s_setOnce = true;
    m_projectName = projectName;
    m_projectVersion = projectVersion;
}
