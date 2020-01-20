//
// Created by dorian on 07. 12. 2019..
//

#include "glfwwindow.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/platform/logger.h"
#include "lava/framework/platform/options/applicationoption.h"
#include "lava/framework/platform/options/windowoptions.h"
#include "lava/framework/utils.h"

void errorCallback(int error, const char *description)
{
    LavaVk::Application::instance->container.resolve<LavaVk::Logger>()->error(
            std::string_format("GLFW Error (code %d): %s", error, description));
}

void LavaVk::GlfwWindow::windowCloseCallback(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void LavaVk::GlfwWindow::windowSizeCallback(GLFWwindow *window, int width, int height)
{
    if (auto glfwWindow = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)))
    {
        glfwWindow->width = width;
        glfwWindow->height = height;
        glfwWindow->onResized(ResizeEventArgs(width, height, glfwWindow->isFullscreen()));
    }
}

void LavaVk::GlfwWindow::windowFocusCallback(GLFWwindow *window, int focused)
{
    if (auto glfwWindow = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)))
    {
        glfwWindow->focused = focused;
        glfwWindow->onFocusChanged(FocusChangedEventArgs(glfwWindow->focused));
    }
}

inline LavaVk::KeyCode translateKeyCode(int key)
{
    static const std::unordered_map<int, LavaVk::KeyCode> keyLookup =
            {
                    {GLFW_KEY_SPACE,         LavaVk::KeyCode::Space},
                    {GLFW_KEY_APOSTROPHE,    LavaVk::KeyCode::Apostrophe},
                    {GLFW_KEY_COMMA,         LavaVk::KeyCode::Comma},
                    {GLFW_KEY_MINUS,         LavaVk::KeyCode::Minus},
                    {GLFW_KEY_PERIOD,        LavaVk::KeyCode::Period},
                    {GLFW_KEY_SLASH,         LavaVk::KeyCode::Slash},
                    {GLFW_KEY_0,             LavaVk::KeyCode::_0},
                    {GLFW_KEY_1,             LavaVk::KeyCode::_1},
                    {GLFW_KEY_2,             LavaVk::KeyCode::_2},
                    {GLFW_KEY_3,             LavaVk::KeyCode::_3},
                    {GLFW_KEY_4,             LavaVk::KeyCode::_4},
                    {GLFW_KEY_5,             LavaVk::KeyCode::_5},
                    {GLFW_KEY_6,             LavaVk::KeyCode::_6},
                    {GLFW_KEY_7,             LavaVk::KeyCode::_7},
                    {GLFW_KEY_8,             LavaVk::KeyCode::_8},
                    {GLFW_KEY_9,             LavaVk::KeyCode::_9},
                    {GLFW_KEY_SEMICOLON,     LavaVk::KeyCode::Semicolon},
                    {GLFW_KEY_EQUAL,         LavaVk::KeyCode::Equal},
                    {GLFW_KEY_A,             LavaVk::KeyCode::A},
                    {GLFW_KEY_B,             LavaVk::KeyCode::B},
                    {GLFW_KEY_C,             LavaVk::KeyCode::C},
                    {GLFW_KEY_D,             LavaVk::KeyCode::D},
                    {GLFW_KEY_E,             LavaVk::KeyCode::E},
                    {GLFW_KEY_F,             LavaVk::KeyCode::F},
                    {GLFW_KEY_G,             LavaVk::KeyCode::G},
                    {GLFW_KEY_H,             LavaVk::KeyCode::H},
                    {GLFW_KEY_I,             LavaVk::KeyCode::I},
                    {GLFW_KEY_J,             LavaVk::KeyCode::J},
                    {GLFW_KEY_K,             LavaVk::KeyCode::K},
                    {GLFW_KEY_L,             LavaVk::KeyCode::L},
                    {GLFW_KEY_M,             LavaVk::KeyCode::M},
                    {GLFW_KEY_N,             LavaVk::KeyCode::N},
                    {GLFW_KEY_O,             LavaVk::KeyCode::O},
                    {GLFW_KEY_P,             LavaVk::KeyCode::P},
                    {GLFW_KEY_Q,             LavaVk::KeyCode::Q},
                    {GLFW_KEY_R,             LavaVk::KeyCode::R},
                    {GLFW_KEY_S,             LavaVk::KeyCode::S},
                    {GLFW_KEY_T,             LavaVk::KeyCode::T},
                    {GLFW_KEY_U,             LavaVk::KeyCode::U},
                    {GLFW_KEY_V,             LavaVk::KeyCode::V},
                    {GLFW_KEY_W,             LavaVk::KeyCode::W},
                    {GLFW_KEY_X,             LavaVk::KeyCode::X},
                    {GLFW_KEY_Y,             LavaVk::KeyCode::Y},
                    {GLFW_KEY_Z,             LavaVk::KeyCode::Z},
                    {GLFW_KEY_LEFT_BRACKET,  LavaVk::KeyCode::LeftBracket},
                    {GLFW_KEY_BACKSLASH,     LavaVk::KeyCode::Backslash},
                    {GLFW_KEY_RIGHT_BRACKET, LavaVk::KeyCode::RightBracket},
                    {GLFW_KEY_GRAVE_ACCENT,  LavaVk::KeyCode::GraveAccent},
                    {GLFW_KEY_ESCAPE,        LavaVk::KeyCode::Escape},
                    {GLFW_KEY_ENTER,         LavaVk::KeyCode::Enter},
                    {GLFW_KEY_TAB,           LavaVk::KeyCode::Tab},
                    {GLFW_KEY_BACKSPACE,     LavaVk::KeyCode::Backspace},
                    {GLFW_KEY_INSERT,        LavaVk::KeyCode::Insert},
                    {GLFW_KEY_DELETE,        LavaVk::KeyCode::DelKey},
                    {GLFW_KEY_RIGHT,         LavaVk::KeyCode::Right},
                    {GLFW_KEY_LEFT,          LavaVk::KeyCode::Left},
                    {GLFW_KEY_DOWN,          LavaVk::KeyCode::Down},
                    {GLFW_KEY_UP,            LavaVk::KeyCode::Up},
                    {GLFW_KEY_PAGE_UP,       LavaVk::KeyCode::PageUp},
                    {GLFW_KEY_PAGE_DOWN,     LavaVk::KeyCode::PageDown},
                    {GLFW_KEY_HOME,          LavaVk::KeyCode::Home},
                    {GLFW_KEY_END,           LavaVk::KeyCode::End},
                    {GLFW_KEY_CAPS_LOCK,     LavaVk::KeyCode::CapsLock},
                    {GLFW_KEY_SCROLL_LOCK,   LavaVk::KeyCode::ScrollLock},
                    {GLFW_KEY_NUM_LOCK,      LavaVk::KeyCode::NumLock},
                    {GLFW_KEY_PRINT_SCREEN,  LavaVk::KeyCode::PrintScreen},
                    {GLFW_KEY_PAUSE,         LavaVk::KeyCode::Pause},
                    {GLFW_KEY_F1,            LavaVk::KeyCode::F1},
                    {GLFW_KEY_F2,            LavaVk::KeyCode::F2},
                    {GLFW_KEY_F3,            LavaVk::KeyCode::F3},
                    {GLFW_KEY_F4,            LavaVk::KeyCode::F4},
                    {GLFW_KEY_F5,            LavaVk::KeyCode::F5},
                    {GLFW_KEY_F6,            LavaVk::KeyCode::F6},
                    {GLFW_KEY_F7,            LavaVk::KeyCode::F7},
                    {GLFW_KEY_F8,            LavaVk::KeyCode::F8},
                    {GLFW_KEY_F9,            LavaVk::KeyCode::F9},
                    {GLFW_KEY_F10,           LavaVk::KeyCode::F10},
                    {GLFW_KEY_F11,           LavaVk::KeyCode::F11},
                    {GLFW_KEY_F12,           LavaVk::KeyCode::F12},
                    {GLFW_KEY_KP_0,          LavaVk::KeyCode::KP_0},
                    {GLFW_KEY_KP_1,          LavaVk::KeyCode::KP_1},
                    {GLFW_KEY_KP_2,          LavaVk::KeyCode::KP_2},
                    {GLFW_KEY_KP_3,          LavaVk::KeyCode::KP_3},
                    {GLFW_KEY_KP_4,          LavaVk::KeyCode::KP_4},
                    {GLFW_KEY_KP_5,          LavaVk::KeyCode::KP_5},
                    {GLFW_KEY_KP_6,          LavaVk::KeyCode::KP_6},
                    {GLFW_KEY_KP_7,          LavaVk::KeyCode::KP_7},
                    {GLFW_KEY_KP_8,          LavaVk::KeyCode::KP_8},
                    {GLFW_KEY_KP_9,          LavaVk::KeyCode::KP_9},
                    {GLFW_KEY_KP_DECIMAL,    LavaVk::KeyCode::KP_Decimal},
                    {GLFW_KEY_KP_DIVIDE,     LavaVk::KeyCode::KP_Divide},
                    {GLFW_KEY_KP_MULTIPLY,   LavaVk::KeyCode::KP_Multiply},
                    {GLFW_KEY_KP_SUBTRACT,   LavaVk::KeyCode::KP_Subtract},
                    {GLFW_KEY_KP_ADD,        LavaVk::KeyCode::KP_Add},
                    {GLFW_KEY_KP_ENTER,      LavaVk::KeyCode::KP_Enter},
                    {GLFW_KEY_KP_EQUAL,      LavaVk::KeyCode::KP_Equal},
                    {GLFW_KEY_LEFT_SHIFT,    LavaVk::KeyCode::LeftShift},
                    {GLFW_KEY_LEFT_CONTROL,  LavaVk::KeyCode::LeftControl},
                    {GLFW_KEY_LEFT_ALT,      LavaVk::KeyCode::LeftAlt},
                    {GLFW_KEY_RIGHT_SHIFT,   LavaVk::KeyCode::RightShift},
                    {GLFW_KEY_RIGHT_CONTROL, LavaVk::KeyCode::RightControl},
                    {GLFW_KEY_RIGHT_ALT,     LavaVk::KeyCode::RightAlt},
            };

    auto keyIt = keyLookup.find(key);

    if (keyIt == keyLookup.end())
    {
        return LavaVk::KeyCode::Unknown;
    }

    return keyIt->second;
}

inline LavaVk::KeyAction translateKeyAction(int action)
{
    if (action == GLFW_PRESS)
    {
        return LavaVk::KeyAction::Down;
    }
    else if (action == GLFW_RELEASE)
    {
        return LavaVk::KeyAction::Up;
    }
    else if (action == GLFW_REPEAT)
    {
        return LavaVk::KeyAction::Repeat;
    }

    return LavaVk::KeyAction::Unknown;
}

void LavaVk::GlfwWindow::keyCallback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
    KeyCode keyCode = translateKeyCode(key);
    KeyAction keyAction = translateKeyAction(action);

    if (auto glfwWindow = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)))
        glfwWindow->onInputReceived(KeyInputEventArgs(keyCode, keyAction));
}

inline LavaVk::MouseButton translateMouseButton(int button)
{
    if (button < GLFW_MOUSE_BUTTON_6)
        return static_cast<LavaVk::MouseButton>(button);

    return LavaVk::MouseButton::Unknown;
}

inline LavaVk::MouseAction translateMouseAction(int action)
{
    if (action == GLFW_PRESS)
        return LavaVk::MouseAction::Down;
    else if (action == GLFW_RELEASE)
        return LavaVk::MouseAction::Up;

    return LavaVk::MouseAction::Unknown;
}


void LavaVk::GlfwWindow::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (auto glfwWindow = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)))
        glfwWindow->onInputReceived(
                MouseButtonInputEventArgs(MouseButton::Unknown, MouseAction::Move, static_cast<float>(xpos),
                                          static_cast<float>(ypos)));

}


void LavaVk::GlfwWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int /*mods*/)
{
    MouseAction mouseAction = translateMouseAction(action);

    if (auto glfwWindow = reinterpret_cast<GlfwWindow *>(glfwGetWindowUserPointer(window)))
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwWindow->onInputReceived(
                MouseButtonInputEventArgs(translateMouseButton(button), mouseAction, static_cast<float>(xpos),
                                          static_cast<float>(ypos)));
    }
}

LavaVk::GlfwWindow::GlfwWindow(LavaVk::DIContainer &container)
{
    if (!glfwInit())
        throw std::runtime_error("GLFW couldn't be initialized.");

    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    auto appOptions = container.option<ApplicationOption>();
    auto windowOptions = container.option<WindowOptions>();
    width = windowOptions->width;
    height = windowOptions->height;
    fullscreenMode = windowOptions->fullscreenMode;
    inputManager = container.resolve<InputManager>();
    logger = container.resolve<Logger>();
    auto title = appOptions->appName + " v" + std::to_string(appOptions->version[0]) + "." +
                 std::to_string(appOptions->version[1]) + "." + std::to_string(appOptions->version[2]) + ".";

    if (fullscreenMode)
    {
        int count;
        GLFWmonitor **monitors = glfwGetMonitors(&count);

        displayId = windowOptions->displayId;
        if (displayId >= static_cast<uint32_t>(count))
        {
            logger->warning("Invalid display id. Using primary display!");
            displayId = 0;
        }
        const GLFWvidmode *mode = glfwGetVideoMode(monitors[displayId]);
        width = mode->width;
        height = mode->height;
        handle = glfwCreateWindow(width, height, title.c_str(), monitors[displayId], nullptr);
    }
    else
        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!handle)
    {
        throw std::runtime_error("Couldn't create glfw window.");
    }

    glfwSetWindowUserPointer(handle, this);

    glfwSetWindowCloseCallback(handle, GlfwWindow::windowCloseCallback);
    glfwSetWindowSizeCallback(handle, GlfwWindow::windowSizeCallback);
    glfwSetWindowFocusCallback(handle, GlfwWindow::windowFocusCallback);
    glfwSetKeyCallback(handle, keyCallback);
    glfwSetCursorPosCallback(handle, cursorPositionCallback);
    glfwSetMouseButtonCallback(handle, mouseButtonCallback);

    glfwSetInputMode(handle, GLFW_STICKY_KEYS, 1);
    glfwSetInputMode(handle, GLFW_STICKY_MOUSE_BUTTONS, 1);
}

LavaVk::GlfwWindow::~GlfwWindow()
{
    glfwDestroyWindow(handle);
    glfwTerminate();

}

vk::SurfaceKHR LavaVk::GlfwWindow::createSurface(const vk::Instance &instance)
{
    if (!handle)
        throw std::runtime_error("Invalid window handle!");

    if (!instance)
        throw std::runtime_error("Invalid instance!");

    VkSurfaceKHR surface;
    VkResult errCode = glfwCreateWindowSurface(instance, handle, nullptr, &surface);

    if (errCode != VK_SUCCESS)
        throw std::runtime_error("Surface creation failed!");

    return vk::SurfaceKHR(surface);
}

bool LavaVk::GlfwWindow::shouldClose() const
{
    return glfwWindowShouldClose(handle);
}

void LavaVk::GlfwWindow::processEvents()
{
    glfwPollEvents();
}

void LavaVk::GlfwWindow::close()
{
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
}

/// It calculates the dpi factor using the density from GLFW physical size
/// <a href="https://www.glfw.org/docs/latest/monitor_guide.html#monitor_size">GLFW docs for dpi</a>
float LavaVk::GlfwWindow::getDpiFactor() const
{
    auto primaryMonitor = glfwGetPrimaryMonitor();
    auto vidmode = glfwGetVideoMode(primaryMonitor);

    int widthMm, heightMm;
    glfwGetMonitorPhysicalSize(primaryMonitor, &widthMm, &heightMm);

    // As suggested by the GLFW monitor guide
    static const float inchToMm = 25.0f;
    static const float winBaseDensity = 96.0f;

    auto dpi = static_cast<uint32_t>(vidmode->width / (widthMm / inchToMm));
    auto dpiFactor = dpi / winBaseDensity;
    return dpiFactor;
}

void LavaVk::GlfwWindow::resize(uint32_t width, uint32_t height)
{
    fullscreenMode = false;
    GlfwWindow::width = width;
    GlfwWindow::height = height;
    glfwSetWindowMonitor(handle, nullptr, GLFW_DONT_CARE, GLFW_DONT_CARE, width, height, GLFW_DONT_CARE);
    onResized(ResizeEventArgs(width, height, false));
}

bool LavaVk::GlfwWindow::isFullscreen()
{
    return fullscreenMode;
}

void LavaVk::GlfwWindow::setFullscreen(bool value)
{
    if (fullscreenMode == value)
        return;

    fullscreenMode = value;

    if (fullscreenMode)
    {
        int count;
        GLFWmonitor **monitors = glfwGetMonitors(&count);

        if (displayId >= static_cast<uint32_t>(count))
        {
            logger->warning("Invalid display id. Using primary display!");
            displayId = 0;
        }
        const GLFWvidmode *mode = glfwGetVideoMode(monitors[displayId]);
        width = mode->width;
        height = mode->height;
        glfwSetWindowMonitor(handle, monitors[displayId], GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE,
                             GLFW_DONT_CARE, GLFW_DONT_CARE);
    }
    else
        glfwSetWindowMonitor(handle, nullptr, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE,
                             GLFW_DONT_CARE, GLFW_DONT_CARE);

    onResized(ResizeEventArgs(width, height, value));
}

uint32_t LavaVk::GlfwWindow::getWidth() const
{
    return width;
}

uint32_t LavaVk::GlfwWindow::getHeight() const
{
    return height;
}

bool LavaVk::GlfwWindow::isFocused() const
{
    return focused;
}

LavaVk::InputManager &LavaVk::GlfwWindow::input()
{
    return *inputManager;
}
