//
// Created by dorian on 08. 12. 2019..
//

#include "linuxwindow.h"
#include <vulkan/vulkan.hpp>

#ifndef VK_KHR_XCB_SURFACE_EXTENSION_NAME
#define VK_KHR_XCB_SURFACE_EXTENSION_NAME "VK_KHR_xcb_surface"
#endif

LavaVk::LinuxWindow::LinuxWindow(LavaVk::DIContainer &container) : GlfwWindow(container)
{
}

std::type_index LavaVk::LinuxWindow::getType() const
{
    return typeid(LinuxWindow);
}

const char *LavaVk::LinuxWindow::getSurfaceExtensions()
{
    return VK_KHR_XCB_SURFACE_EXTENSION_NAME;
}
