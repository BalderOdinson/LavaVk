//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_BUFFEROPTIONS_H
#define LAVAVK_BUFFEROPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/core/allocator.h"
#include "allocationoption.h"

namespace LavaVk::Core
{
    class BufferOptions : public AllocationOption
    {
    public:
        BufferOptions() = default;
        BufferOptions(vk::DeviceSize size, vk::DeviceSize n, const vk::BufferUsageFlags &bufferUsage, vma::MemoryUsage memoryUsage, bool isMapped,
                      bool canGetLost = false,
                      const vk::MemoryPropertyFlags &requiredFlags = {},
                      const vk::MemoryPropertyFlags &preferredFlags = {});

        /// Size of single element in bytes.
        vk::DeviceSize size;
        /// Number of elements.
        vk::DeviceSize n;
        vk::BufferUsageFlags bufferUsage;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_BUFFEROPTIONS_H
