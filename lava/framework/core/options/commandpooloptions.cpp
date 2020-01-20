//
// Created by dorian on 09. 12. 2019..
//

#include "commandpooloptions.h"

LavaVk::Core::CommandPoolOptions::CommandPoolOptions(LavaVk::Core::CommandBuffer::ResetMode resetMode,
                                                     bool releaseOnReset, uint32_t queueFamilyIndex,
                                                     uint32_t threadIndex, SharedRenderFrame frame)
        : resetMode(resetMode), queueFamilyIndex(queueFamilyIndex),
          releaseOnReset(releaseOnReset), threadIndex(threadIndex), frame(frame)
{
}

std::type_index LavaVk::Core::CommandPoolOptions::getType() const
{
    return typeid(CommandPoolOptions);
}
