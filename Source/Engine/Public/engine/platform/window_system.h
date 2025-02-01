#pragma once
#include <engine/export_macros.h>
#include <engine/result.h>
#include <engine/platform/input_system.h>

typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWmonitor GLFWmonitor;
typedef struct GLFWvidmode GLFWvidmode;
typedef struct VkInstance_T *VkInstance;
typedef struct VkSurfaceKHR_T *VkSurfaceKHR;

namespace engine {
class ENGINEAPI WindowSystem {
    static constexpr uint32_t minWindowWidth = 640;
    static constexpr uint32_t minWindowHeight = 480;
    WindowSystem() = default;
    WindowSystem(const WindowSystem &) = delete;
    WindowSystem &operator=(const WindowSystem &) = delete;
    GLFWwindow *m_pWindow = nullptr;
    GLFWmonitor *m_pMonitor = nullptr;
    const GLFWvidmode *m_pVidMode = nullptr;
    bool m_alive = false;
    bool m_framebufferSizeChangedInLastFrame = false;
    uint32_t m_width = 0, m_height = 0;
    uint32_t m_framebufferWidth = 0, m_framebufferHeight = 0;
    uint32_t m_screenWidth = 0, m_screenHeight = 0;
    bool m_borderless = false;
    bool m_fullscreen = false;
    EMouseMode m_internalMouseMode = EMouseMode::Free;
    friend struct WindowCallbacks;

    void change_window_size();
    void change_window_style();
    void change_window_monitor();

public:
    static WindowSystem &instance();

    EResult init();
    void update();
    void destroy();

    constexpr bool is_alive() const { return m_alive; }
    constexpr bool is_framebuffer_size_changed() const { return m_framebufferSizeChangedInLastFrame; }
    constexpr uint32_t get_width() const { return m_width; }
    constexpr uint32_t get_height() const { return m_height; }
    constexpr uint32_t get_framebuffer_width() const { return m_framebufferWidth; }
    constexpr uint32_t get_framebuffer_height() const { return m_framebufferHeight; }
    constexpr uint32_t get_screen_width() const { return m_screenWidth; }
    constexpr uint32_t get_screen_height() const { return m_screenHeight; }
    constexpr bool is_borderless() const { return m_borderless; }
    constexpr bool is_fullscreen() const { return m_fullscreen; }

    int create_vulkan_surface(VkInstance pInstance, VkSurfaceKHR *ppSurface) const;

    constexpr void set_size(uint32_t width, uint32_t height) {
        m_width = width;
        m_height = height;
        if (m_alive && m_pWindow) {
            change_window_size();
        }
    }
    constexpr void set_borderless(bool borderless) {
        m_borderless = borderless;
        if (m_alive && m_pWindow) {
            change_window_style();
        }
    }
    constexpr void set_fullscreen(bool fullscreen) {
        m_fullscreen = fullscreen;
        if (m_alive && m_pWindow) {
            change_window_style();
        }
    }
    constexpr void close() { m_alive = false; }
};

inline auto &window_system() { return WindowSystem::instance(); }
} // namespace engine
