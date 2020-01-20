//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_WINDOWOPTIONS_H
#define LAVAVK_WINDOWOPTIONS_H

#include "lava/framework/object.h"

namespace LavaVk
{
    class WindowOptions : public virtual Object
    {
    public:
        uint32_t height;
        uint32_t width;
        bool fullscreenMode;
        uint32_t displayId{0};
        std::type_index getType() const override;
    };
}


#endif //LAVAVK_WINDOWOPTIONS_H
