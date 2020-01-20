//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_LINUXAPPLICATION_H
#define LAVAVK_LINUXAPPLICATION_H

#include "lava/framework/platform/application.h"

namespace LavaVk
{
    class LinuxApplication : public Application
    {
    protected:
        void installConfigurations(DIContainer &container) override;

        Window *createWindow(DIContainer &container) override;
    };
}


#endif //LAVAVK_LINUXAPPLICATION_H
