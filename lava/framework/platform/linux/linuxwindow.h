//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_LINUXWINDOW_H
#define LAVAVK_LINUXWINDOW_H

#include "lava/framework/platform/shared/glfwwindow.h"

namespace LavaVk
{
    class LinuxWindow : public GlfwWindow
    {
    public:
        explicit LinuxWindow(DIContainer &container);
        std::type_index getType() const override;
        const char *getSurfaceExtensions() override;
    };
}


#endif //LAVAVK_LINUXWINDOW_H
