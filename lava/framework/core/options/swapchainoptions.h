//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_SWAPCHAINOPTIONS_H
#define LAVAVK_SWAPCHAINOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class SwapchainOptions : public virtual Object
    {
    public:
        explicit SwapchainOptions(vk::Extent2D extent = {}, int32_t imageCount = 3,
                         vk::SurfaceTransformFlagBitsKHR transform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
                         vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo,
                         const vk::ImageUsageFlags& imageUsage = vk::ImageUsageFlagBits::eColorAttachment |
                                                          vk::ImageUsageFlagBits::eTransferSrc);

        vk::Extent2D extent{};
        uint32_t imageCount{3};
        vk::SurfaceTransformFlagBitsKHR transform{vk::SurfaceTransformFlagBitsKHR::eIdentity};
        vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};
        vk::ImageUsageFlags imageUsage{vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc};

        std::type_index getType() const override;
    };

    using UniqueSwapchainOptions = std::unique_ptr<SwapchainOptions>;
    using SharedSwapchainOptions = std::shared_ptr<SwapchainOptions>;
    using WeakSwapchainOptions = std::weak_ptr<SwapchainOptions>;
}


#endif //LAVAVK_SWAPCHAINOPTIONS_H
