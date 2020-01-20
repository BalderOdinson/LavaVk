//
// Created by dorian on 10. 12. 2019..
//

#include "image2buffertransfer.h"
#include "commandbuffer.h"

#include <utility>

LavaVk::Core::Image2BufferTransfer::Image2BufferTransfer(LavaVk::Core::SharedCommandBuffer cmd,
                                                         LavaVk::Core::SharedImageObject src,
                                                         LavaVk::Core::SharedBuffer dst) : cmd(std::move(cmd)),
                                                                                           src(std::move(src)),
                                                                                           dst(std::move(dst))
{
}

LavaVk::Core::Image2BufferTransfer &
LavaVk::Core::Image2BufferTransfer::addRegion(vk::DeviceSize bufferOffset, vk::DeviceSize bufferRowLength,
                                              vk::DeviceSize bufferImageHeight, const vk::ImageAspectFlags &aspectMask,
                                              uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount,
                                              vk::Offset3D imageOffset, vk::Extent3D imageExtent)
{
    regions.emplace_back(bufferOffset, bufferRowLength, bufferImageHeight,
                         vk::ImageSubresourceLayers{aspectMask, mipLevel, baseArrayLayer, layerCount}, imageOffset,
                         imageExtent);
    return *this;

}

LavaVk::Core::Image2BufferTransfer &
LavaVk::Core::Image2BufferTransfer::addRegion(vk::DeviceSize bufferOffset, const vk::ImageAspectFlags &aspectMask,
                                              uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount,
                                              vk::Offset3D imageOffset, vk::Extent3D imageExtent)
{
    regions.emplace_back(bufferOffset, 0, 0,
                         vk::ImageSubresourceLayers{aspectMask, mipLevel, baseArrayLayer, layerCount}, imageOffset,
                         imageExtent);
    return *this;

}

LavaVk::Core::Image2BufferTransfer &LavaVk::Core::Image2BufferTransfer::setSourceImageLayout(vk::ImageLayout layout)
{
    srcLayout = layout;
    return *this;
}

void LavaVk::Core::Image2BufferTransfer::execute()
{
    cmd->getHandle().copyImageToBuffer(src->getHandle(), srcLayout, dst->getHandle(), regions);
}
