//
// Created by dorian on 13. 12. 2019..
//

#include "devicememoryupdateoptions.h"

#include <utility>

LavaVk::Core::DeviceMemoryUpdateOptions::DeviceMemoryUpdateOptions(LavaVk::Core::SharedQueue queue,
                                                                   LavaVk::Core::SharedFence fence,
                                                                   LavaVk::Core::SharedCommandPool pool) : queue(
        std::move(queue)), fence(std::move(fence)), pool(std::move(pool))
{

}

LavaVk::Core::DeviceMemoryUpdateOptions::DeviceMemoryUpdateOptions(LavaVk::Core::SharedCommandBuffer buffer)
        : commandBuffer(std::move(buffer))
{
}

std::type_index LavaVk::Core::DeviceMemoryUpdateOptions::getType() const
{
    return typeid(DeviceMemoryUpdateOptions);
}
