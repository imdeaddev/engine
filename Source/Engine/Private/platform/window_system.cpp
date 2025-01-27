#include <volk.h>
#include <engine/platform/window_system.h>
#include <engine/platform/project_system.h>
#include <GLFW/glfw3.h>

struct engine::WindowCallbacks {
    static void glfwCloseCallback(GLFWwindow *pGlfw) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            pWindow->m_alive = false;
        }
    }
};

void engine::WindowSystem::change_window_size() {}
void engine::WindowSystem::change_window_style() {}
void engine::WindowSystem::change_window_monitor() {}

engine::WindowSystem &engine::WindowSystem::instance() {
    static WindowSystem s_system{};
    return s_system;
}

int engine::WindowSystem::create_vulkan_surface(VkInstance pInstance, VkSurfaceKHR *ppSurface) const {
    return glfwCreateWindowSurface(pInstance, m_pWindow, nullptr, ppSurface);
}

engine::EResult engine::WindowSystem::init() {
    if (m_pWindow) {
        return EResult::Success;
    }
    if (!glfwInit()) {
        return EResult::APIError;
    }

    m_pMonitor = glfwGetPrimaryMonitor();
    m_pVidMode = glfwGetVideoMode(m_pMonitor);
    if (m_pVidMode == nullptr) {
        return EResult::APIError;
    }
    m_screenWidth = m_pVidMode->width;
    m_screenHeight = m_pVidMode->height;

    glfwInitHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwInitHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwInitHint(GLFW_DECORATED, !m_borderless);

    m_pWindow =
        glfwCreateWindow(m_width, m_height, project_system().get_project_name().c_str(), m_fullscreen ? m_pMonitor : nullptr, nullptr);

    if (!m_pWindow) {
        return EResult::APIError;
    }

    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowCloseCallback(m_pWindow, WindowCallbacks::glfwCloseCallback);
    m_alive = true;

    return EResult::Success;
}
void engine::WindowSystem::update() { glfwPollEvents(); }
void engine::WindowSystem::destroy() {
    if (m_pWindow) {
        glfwDestroyWindow(m_pWindow);
    }
    glfwTerminate();
}
