//
// Created by oshikuru on 13. 01. 2020..
//

#include "win32window.h"
#include <vulkan/vulkan.hpp>

#ifndef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
#endif

LavaVk::Win32Window::Win32Window(LavaVk::DIContainer &container) : GlfwWindow(container)
{}

std::type_index LavaVk::Win32Window::getType() const
{
    return typeid(Win32Window);
}

const char *LavaVk::Win32Window::getSurfaceExtensions()
{
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}
