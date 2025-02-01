#pragma once
#include <engine/export_macros.h>
#include <glm/vec2.hpp>
#include <unordered_map>

namespace engine {
enum class EKeyCode {
    Unknown = -1,
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    Number0 = 48,
    Number1 = 49,
    Number2 = 50,
    Number3 = 51,
    Number4 = 52,
    Number5 = 53,
    Number6 = 54,
    Number7 = 55,
    Number8 = 56,
    Number9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    World1 = 161,
    World2 = 162,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    Keypad0 = 320,
    Keypad1 = 321,
    Keypad2 = 322,
    Keypad3 = 323,
    Keypad4 = 324,
    Keypad5 = 325,
    Keypad6 = 326,
    Keypad7 = 327,
    Keypad8 = 328,
    Keypad9 = 329,
    KeypadDecimal = 330,
    KeypadDivide = 331,
    KeypadMultiply = 332,
    KeypadSubtract = 333,
    KeypadAdd = 334,
    KeypadEnter = 335,
    KeypadEqual = 336,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,
    Mouse0,
    Mouse1,
    Mouse2,
    Mouse3,
    Mouse4,
    Mouse5,
    Mouse6,
    Mouse7,
    Count,

    LeftMouseButton = Mouse0,
    RightMouseButton = Mouse1,
    MiddleMouseButton = Mouse2,
};

enum class EMouseMode { Free, Locked, Invisible };

class ENGINEAPI InputSystem {
    enum KeyState : uint8_t {
        KS_NONE = 0,
        KS_PRESSED = 0x1,
        KS_RELEASED = 0x2,
        KS_HAS_INPUT_IN_THIS_FRAME = 0x4,
        KS_JUST_PRESSED = KS_PRESSED | KS_HAS_INPUT_IN_THIS_FRAME,
        KS_JUST_RELEASED = KS_RELEASED | KS_HAS_INPUT_IN_THIS_FRAME,
    };
    InputSystem();
    InputSystem(const InputSystem &) = delete;
    InputSystem &operator=(const InputSystem &) = delete;
    friend struct WindowCallbacks;
    friend class WindowSystem;
    void reset();

    glm::vec2 m_mousePos{};
    glm::vec2 m_lastMousePos{};
    glm::vec2 m_mouseDelta{};
    glm::vec2 m_scrollDelta{};
    EMouseMode m_mouseMode = EMouseMode::Free;
    std::unordered_map<EKeyCode, KeyState> m_keyStates{};

public:
    static InputSystem &instance();

    constexpr const glm::vec2 &get_mouse_position() const { return m_mousePos; }
    constexpr const glm::vec2 &get_mouse_delta() const { return m_mouseDelta; }
    constexpr const glm::vec2 &get_mouse_scroll() const { return m_scrollDelta; }

    constexpr void set_mouse_mode(EMouseMode mode) { m_mouseMode = mode; }

    constexpr bool is_key_just_pressed(EKeyCode kc) const { return (m_keyStates.at(kc) & KS_JUST_PRESSED) == KS_JUST_PRESSED; }
    constexpr bool is_key_pressed(EKeyCode kc) const { return (m_keyStates.at(kc) & KS_PRESSED) == KS_PRESSED; }
    constexpr bool is_key_just_released(EKeyCode kc) const { return (m_keyStates.at(kc) & KS_JUST_RELEASED) == KS_JUST_RELEASED; }
    constexpr bool is_key_released(EKeyCode kc) const { return (m_keyStates.at(kc) & KS_RELEASED) == KS_RELEASED; }
};

inline InputSystem &input_system() { return InputSystem::instance(); }
} // namespace engine
