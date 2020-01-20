//
// Created by dorian on 10. 12. 2019..
//

#include "buffer2imagetransfer.h"
#include "commandbuffer.h"

#include <utility>

LavaVk::Core::Buffer2ImageTransfer::Buffer2ImageTransfer(LavaVk::Core::SharedCommandBuffer cmd,
                                                         LavaVk::Core::SharedBuffer src,
                                                         LavaVk::Core::SharedImageObject dst) : cmd(std::move(cmd)),
                                                                                                src(std::move(src)),
                                                                                                dst(std::move(dst))
{
}

LavaVk::Core::Buffer2ImageTransfer &
LavaVk::Core::Buffer2ImageTransfer::addRegion(vk::DeviceSize bufferOffset, vk::DeviceSize bufferRowLength,
                                              vk::DeviceSize bufferImageHeight, const vk::ImageAspectFlags &aspectMask,
                                              uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount,
                                              vk::Offset3D imageOffset, vk::Extent3D imageExtent)
{
    regions.emplace_back(bufferOffset, bufferRowLength, bufferImageHeight,
                         vk::ImageSubresourceLayers{aspectMask, mipLevel, baseArrayLayer, layerCount}, imageOffset,
                         imageExtent);
    return *this;

}

LavaVk::Core::Buffer2ImageTransfer &
LavaVk::Core::Buffer2ImageTransfer::addRegion(vk::DeviceSize bufferOffset, const vk::ImageAspectFlags &aspectMask,
                                              uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount,
                                              vk::Offset3D imageOffset, vk::Extent3D imageExtent)
{
    regions.emplace_back(bufferOffset, 0, 0, vk::ImageSubresourceLayers{aspectMask, mipLevel, baseArrayLayer, layerCount}, imageOffset,
                         imageExtent);
    return *this;

}

LavaVk::Core::Buffer2ImageTransfer &
LavaVk::Core::Buffer2ImageTransfer::setDestinationImageLayout(vk::ImageLayout layout)
{
    dstLayout = layout;
    return *this;

}

void LavaVk::Core::Buffer2ImageTransfer::execute()
{
    cmd->getHandle().copyBufferToImage(src->getHandle(), dst->getHandle(), dstLayout, regions);
}
