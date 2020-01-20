//
// Created by oshikuru on 13. 01. 2020..
//

#ifndef LAVAVK_WIN32WINDOW_H
#define LAVAVK_WIN32WINDOW_H

#include "lava/framework/platform/shared/glfwwindow.h"

namespace LavaVk
{
    class Win32Window : public GlfwWindow
    {
    public:
        explicit Win32Window(DIContainer &container);
        std::type_index getType() const override;
        const char *getSurfaceExtensions() override;
    };
}

#endif //LAVAVK_WIN32WINDOW_H
