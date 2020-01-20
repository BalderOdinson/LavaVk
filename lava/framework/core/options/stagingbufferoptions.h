//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_STAGINGBUFFEROPTIONS_H
#define LAVAVK_STAGINGBUFFEROPTIONS_H

#include "bufferoptions.h"

namespace LavaVk::Core
{
    class StagingBufferOptions : public BufferOptions
    {
    public:
        StagingBufferOptions(vk::DeviceSize size, vk::DeviceSize n);
    };
}


#endif //LAVAVK_STAGINGBUFFEROPTIONS_H
