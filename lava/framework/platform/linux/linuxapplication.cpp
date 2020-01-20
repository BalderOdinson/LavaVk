//
// Created by dorian on 08. 12. 2019..
//

#include "lava/framework/constants.h"
#include "linuxapplication.h"
#include "linuxwindow.h"

LavaVk::Window *LavaVk::LinuxApplication::createWindow(LavaVk::DIContainer &container)
{
    return new LinuxWindow(container);
}

void LavaVk::LinuxApplication::installConfigurations(LavaVk::DIContainer &container)
{
    Application::installConfigurations(container);
    container.option<Core::SwapchainOptions>()->presentMode = vk::PresentModeKHR::eImmediate;
}
