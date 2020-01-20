//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_BUFFERPOOLOPTIONS_H
#define LAVAVK_BUFFERPOOLOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/third-party/vma.h"
#include "lava/framework/object.h"

namespace LavaVk
{
    class BufferPoolOptions : public virtual Object
    {
    public:
        BufferPoolOptions() = default;
        BufferPoolOptions(vk::DeviceSize blockSize, const vk::BufferUsageFlags& usage,
                          vma::MemoryUsage memoryUsage = vma::MemoryUsage::eCpuToGpu);

        vk::DeviceSize blockSize{};
        vk::BufferUsageFlags usage;
        vma::MemoryUsage memoryUsage = vma::MemoryUsage::eCpuToGpu;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_BUFFERPOOLOPTIONS_H
