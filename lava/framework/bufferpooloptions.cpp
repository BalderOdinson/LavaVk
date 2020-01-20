//
// Created by dorian on 12. 12. 2019..
//

#include "bufferpooloptions.h"

LavaVk::BufferPoolOptions::BufferPoolOptions(vk::DeviceSize blockSize, const vk::BufferUsageFlags &usage,
                                                   vma::MemoryUsage memoryUsage) : blockSize(blockSize), usage(usage),
                                                                                   memoryUsage(memoryUsage)
{
}

std::type_index LavaVk::BufferPoolOptions::getType() const
{
    return typeid(BufferPoolOptions);
}
