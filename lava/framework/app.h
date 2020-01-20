//
// Created by oshikuru on 13. 01. 2020..
//

#ifndef LAVAVK_APP_H
#define LAVAVK_APP_H

#include <vulkan/vulkan.hpp>
#ifdef __linux__
#include "platform/linux/linuxapplication.h"
namespace LavaVk
{
    using App = LinuxApplication;
}
#elif _WIN32
#include "platform/windows/windowsapplication.h"
namespace LavaVk
{
    using App = WindowsApplication;
}
#endif

#endif //LAVAVK_APP_H
