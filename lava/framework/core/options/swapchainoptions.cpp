//
// Created by dorian on 10. 12. 2019..
//

#include "swapchainoptions.h"

std::type_index LavaVk::Core::SwapchainOptions::getType() const
{
    return typeid(SwapchainOptions);
}

LavaVk::Core::SwapchainOptions::SwapchainOptions(vk::Extent2D extent, int32_t imageCount,
                                                 vk::SurfaceTransformFlagBitsKHR transform,
                                                 vk::PresentModeKHR presentMode, const vk::ImageUsageFlags& imageUsage)
        : extent(extent), imageCount(imageCount), transform(transform), presentMode(presentMode), imageUsage(imageUsage)
{
}
