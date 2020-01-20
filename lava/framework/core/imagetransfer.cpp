//
// Created by dorian on 17. 12. 2019..
//

#include "imagetransfer.h"
#include "commandbuffer.h"

LavaVk::Core::ImageTransfer::ImageTransfer(LavaVk::Core::SharedCommandBuffer cmd, LavaVk::Core::SharedImageObject src,
                                           LavaVk::Core::SharedImageObject dst) : cmd(std::move(cmd)),
                                                                                  src(std::move(src)),
                                                                                  dst(std::move(dst))
{

}

LavaVk::Core::ImageTransfer &
LavaVk::Core::ImageTransfer::addRegion(const vk::ImageAspectFlags &srcAspectMask, uint32_t srcMipLevel,
                                       uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, vk::Offset3D srcImageOffset,
                                       const vk::ImageAspectFlags &dstAspectMask, uint32_t dstMipLevel,
                                       uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, vk::Offset3D dstImageOffset,
                                       vk::Extent3D imageExtent)
{
    regions.emplace_back(
            vk::ImageSubresourceLayers(srcAspectMask, srcMipLevel, srcBaseArrayLayer, srcLayerCount), srcImageOffset,
            vk::ImageSubresourceLayers(dstAspectMask, dstMipLevel, dstBaseArrayLayer, dstLayerCount), dstImageOffset,
            imageExtent);

    return *this;
}

LavaVk::Core::ImageTransfer &LavaVk::Core::ImageTransfer::setSourceImageLayout(vk::ImageLayout layout)
{
    srcLayout = layout;
    return *this;
}

LavaVk::Core::ImageTransfer &LavaVk::Core::ImageTransfer::setDestinationImageLayout(vk::ImageLayout layout)
{
    dstLayout = layout;
    return *this;
}

void LavaVk::Core::ImageTransfer::execute()
{
    cmd->getHandle().copyImage(src->getHandle(), srcLayout, dst->getHandle(), dstLayout, regions);
}
