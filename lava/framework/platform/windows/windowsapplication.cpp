//
// Created by oshikuru on 13. 01. 2020..
//

#include "windowsapplication.h"
#include "win32window.h"

LavaVk::Window *LavaVk::WindowsApplication::createWindow(LavaVk::DIContainer &container)
{
    return new Win32Window(container);
}
