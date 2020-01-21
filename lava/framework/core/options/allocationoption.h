//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_ALLOCATIONOPTION_H
#define LAVAVK_ALLOCATIONOPTION_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/core/allocator.h"
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class AllocationOption : public virtual Object
    {
    public:
        AllocationOption() = default;
        AllocationOption(vma::MemoryUsage usage, bool isMapped, bool canGetLost = false,
                         const vk::MemoryPropertyFlags& requiredFlags = {}, const vk::MemoryPropertyFlags& preferredFlags = {});

        vma::MemoryUsage memoryUsage{};
        bool isMapped{false};
        bool canGetLost{false};
        vk::MemoryPropertyFlags requiredFlags{};
        vk::MemoryPropertyFlags preferredFlags{};
    };
}

#endif //LAVAVK_ALLOCATIONOPTION_H
