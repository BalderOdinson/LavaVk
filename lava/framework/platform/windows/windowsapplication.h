//
// Created by oshikuru on 13. 01. 2020..
//

#ifndef LAVAVK_WINDOWSAPPLICATION_H
#define LAVAVK_WINDOWSAPPLICATION_H

#include "lava/framework/platform/application.h"

namespace LavaVk
{
    class WindowsApplication : public Application
    {
    protected:
        Window *createWindow(DIContainer &container) override;
    };
}


#endif //LAVAVK_WINDOWSAPPLICATION_H
