//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_GLFWWINDOW_H
#define LAVAVK_GLFWWINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "lava/framework/platform/window.h"
#include "lava/framework/platform/logger.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"


namespace LavaVk
{
    class GlfwWindow : public Window
    {
    public:
        explicit GlfwWindow(DIContainer &container);
        ~GlfwWindow() override;

        vk::SurfaceKHR createSurface(const vk::Instance &instance) override;
        bool shouldClose() const override;
        void processEvents() override;
        void close() override;
        float getDpiFactor() const override;
        void resize(uint32_t width, uint32_t height) override;
        bool isFullscreen() override;
        void setFullscreen(bool value) override;
        uint32_t getWidth() const override;
        uint32_t getHeight() const override;
        bool isFocused() const override;

    protected:
        InputManager &input() override;

    private:
        static void windowCloseCallback(GLFWwindow *window);
        static void windowSizeCallback(GLFWwindow *window, int width, int height);
        static void windowFocusCallback(GLFWwindow *window, int focused);
        static void keyCallback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/);
        static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int /*mods*/);

        GLFWwindow *handle{nullptr};
        uint32_t width;
        uint32_t height;
        size_t displayId;
        bool fullscreenMode;
        bool focused{true};
        SharedInputManager inputManager;
        SharedLogger logger;
    };
}


#endif //LAVAVK_GLFWWINDOW_H
