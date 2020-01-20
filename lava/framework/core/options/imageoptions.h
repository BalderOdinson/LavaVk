//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGEOPTIONS_H
#define LAVAVK_IMAGEOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/third-party/vma.h"
#include "allocationoption.h"

namespace LavaVk::Core
{
    class ImageOptions : public AllocationOption
    {
    public:
        ImageOptions() = default;
        ImageOptions(vma::MemoryUsage memoryUsage, bool isMapped, vk::Extent3D extent, vk::Format format,
                     const vk::ImageUsageFlags& imageUsage, bool canGetLost = false,
                     const vk::MemoryPropertyFlags &requiredFlags = {},
                     const vk::MemoryPropertyFlags &preferredFlags = {},
                     vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1, uint32_t mipLevels = 1,
                     uint32_t arrayLayers = 1, vk::ImageTiling imageTiling = vk::ImageTiling::eOptimal, vk::ImageCreateFlags flags = {});

        vk::Extent3D extent;
        vk::Format format;
        vk::ImageUsageFlags imageUsage;
        vk::SampleCountFlagBits sampleCount{vk::SampleCountFlagBits::e1};
        uint32_t mipLevels{1};
        uint32_t arrayLayers{1};
        vk::ImageTiling imageTiling{vk::ImageTiling::eOptimal};
        vk::ImageCreateFlags flags{};

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_IMAGEOPTIONS_H
