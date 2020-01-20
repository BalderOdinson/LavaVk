//
// Created by dorian on 09. 12. 2019..
//

#include "bufferoptions.h"

LavaVk::Core::BufferOptions::BufferOptions(vk::DeviceSize size, vk::DeviceSize n, const vk::BufferUsageFlags &bufferUsage,
                                           vma::MemoryUsage memoryUsage, bool isMapped, bool canGetLost,
                                           const vk::MemoryPropertyFlags &requiredFlags,
                                           const vk::MemoryPropertyFlags &preferredFlags) : AllocationOption(
        memoryUsage, isMapped, canGetLost, requiredFlags, preferredFlags), size(size), n(n), bufferUsage(bufferUsage)
{
}

std::type_index LavaVk::Core::BufferOptions::getType() const
{
    return typeid(BufferOptions);
}
