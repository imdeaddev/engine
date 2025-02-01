#include <algorithm>
#include <volk.h>
#include <engine/platform/window_system.h>
#include <engine/platform/project_system.h>
#include <GLFW/glfw3.h>

struct engine::WindowCallbacks {
    static void glfw_close_callback(GLFWwindow *pGlfw) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            pWindow->m_alive = false;
        }
    }

    static void glfw_size_callback(GLFWwindow *pGlfw, int w, int h) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            pWindow->m_width = w;
            pWindow->m_height = h;
        }
    }

    static void glfw_framebuffer_size_callback(GLFWwindow *pGlfw, int w, int h) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            pWindow->m_framebufferWidth = w;
            pWindow->m_framebufferHeight = h;
            pWindow->m_framebufferSizeChangedInLastFrame = true;
        }
    }

    /*
        static void glfw_callback_ref(GLFWwindow *pGlfw) {
            auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
            if (pWindow) {
            }
        }
     */

    // Fuck SOLID priciples :)
    static void glfw_mouse_move(GLFWwindow *pGlfw, double x, double y) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            auto &input = input_system();
            if (pWindow->m_internalMouseMode == EMouseMode::Locked) {
                input.m_mouseDelta.x = x;
                input.m_mouseDelta.y = y;
                input.m_mousePos.x = 0;
                input.m_mousePos.y = 0;
            } else {
                input.m_lastMousePos = input.m_mousePos;
                input.m_mousePos.x = x;
                input.m_mousePos.y = y;
                input.m_mouseDelta = input.m_mousePos - input.m_lastMousePos;
            }
        }
    }

    static void glfw_mouse_scroll(GLFWwindow *pGlfw, double x, double y) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            auto &input = input_system();
            input.m_scrollDelta.x = x;
            input.m_scrollDelta.y = y;
        }
    }

    static void glfw_key_callback(GLFWwindow *pGlfw, int keycode, int scancode, int action, int mods) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            if (action == GLFW_PRESS) {
                input_system().m_keyStates[static_cast<EKeyCode>(keycode)] = InputSystem::KS_JUST_PRESSED;
            } else if (action == GLFW_RELEASE) {
                input_system().m_keyStates[static_cast<EKeyCode>(keycode)] = InputSystem::KS_JUST_RELEASED;
            }
        }
    }

    static void glfw_mouse_button_callback(GLFWwindow *pGlfw, int buttonIdx, int action, int mods) {
        auto *pWindow = reinterpret_cast<engine::WindowSystem *>(glfwGetWindowUserPointer(pGlfw));
        if (pWindow) {
            if (action == GLFW_PRESS) {
                input_system().m_keyStates[static_cast<EKeyCode>(static_cast<int>(EKeyCode::Mouse0) + buttonIdx)] =
                    InputSystem::KS_JUST_PRESSED;
            } else if (action == GLFW_RELEASE) {
                input_system().m_keyStates[static_cast<EKeyCode>(static_cast<int>(EKeyCode::Mouse0) + buttonIdx)] =
                    InputSystem::KS_JUST_RELEASED;
            }
        }
    }
};

constexpr uint32_t clamp_window_size(uint32_t size, uint32_t min_, uint32_t max_) {
    if (size == 0) {
        return max_;
    }
    return std::clamp(size, min_, max_);
}

void engine::WindowSystem::change_window_size() {
    m_width = clamp_window_size(m_width, minWindowWidth, m_screenWidth);
    m_height = clamp_window_size(m_height, minWindowHeight, m_screenHeight);
    glfwSetWindowSize(m_pWindow, m_width, m_height);
}
void engine::WindowSystem::change_window_style() { glfwSetWindowAttrib(m_pWindow, GLFW_DECORATED, !m_borderless); }
void engine::WindowSystem::change_window_monitor() {
    if (m_fullscreen) {
        glfwSetWindowMonitor(m_pWindow, m_pMonitor, 0, 0, m_width, m_height, GLFW_DONT_CARE);
    } else {
        glfwSetWindowMonitor(m_pWindow, nullptr, (m_screenWidth - m_width) / 2, (m_screenHeight - m_height) / 2, m_width, m_height,
                             GLFW_DONT_CARE);
    }
}

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
    m_width = clamp_window_size(m_width, minWindowWidth, m_screenWidth);
    m_height = clamp_window_size(m_height, minWindowHeight, m_screenHeight);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, !m_borderless);

    m_pWindow =
        glfwCreateWindow(m_width, m_height, project_system().get_project_name().c_str(), m_fullscreen ? m_pMonitor : nullptr, nullptr);

    if (!m_pWindow) {
        return EResult::APIError;
    }
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(m_pWindow, &fbw, &fbh);
    m_framebufferWidth = fbw;
    m_framebufferHeight = fbh;

    { // Set initial data for cursor
        double x = 0, y = 0;
        glfwGetCursorPos(m_pWindow, &x, &y);
        auto &input = input_system();
        input.m_lastMousePos.x = x;
        input.m_lastMousePos.y = y;
        input.m_mousePos = input.m_lastMousePos;
    }

    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowCloseCallback(m_pWindow, WindowCallbacks::glfw_close_callback);
    glfwSetWindowSizeCallback(m_pWindow, WindowCallbacks::glfw_size_callback);
    glfwSetFramebufferSizeCallback(m_pWindow, WindowCallbacks::glfw_framebuffer_size_callback);
    glfwSetKeyCallback(m_pWindow, WindowCallbacks::glfw_key_callback);
    glfwSetMouseButtonCallback(m_pWindow, WindowCallbacks::glfw_mouse_button_callback);
    glfwSetCursorPosCallback(m_pWindow, WindowCallbacks::glfw_mouse_move);
    glfwSetScrollCallback(m_pWindow, WindowCallbacks::glfw_mouse_scroll);

    m_alive = true;

    return EResult::Success;
}
void engine::WindowSystem::update() {
    engine::input_system().reset();
    m_framebufferSizeChangedInLastFrame = false;
    if (m_internalMouseMode != engine::input_system().m_mouseMode) {
        m_internalMouseMode = engine::input_system().m_mouseMode;
        switch (m_internalMouseMode) {
        case EMouseMode::Free:
            glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case EMouseMode::Locked:
            glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwRawMouseMotionSupported()) {
                glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }
            break;
        case EMouseMode::Invisible:
            glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
    }
    glfwPollEvents();
}
void engine::WindowSystem::destroy() {
    if (m_pWindow) {
        glfwDestroyWindow(m_pWindow);
    }
    glfwTerminate();
}
