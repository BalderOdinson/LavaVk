//
// Created by dorian on 17. 12. 2019..
//

#ifndef LAVAVK_IMAGETRANSFER_H
#define LAVAVK_IMAGETRANSFER_H

#include <vulkan/vulkan.hpp>
#include "imageobject.h"

namespace LavaVk::Core
{
    class CommandBuffer;

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    class ImageTransfer
    {
    public:
        ImageTransfer(SharedCommandBuffer cmd, SharedImageObject src, SharedImageObject dst);

        ImageTransfer &
        addRegion(const vk::ImageAspectFlags &srcAspectMask, uint32_t srcMipLevel,
                  uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, vk::Offset3D srcImageOffset,
                  const vk::ImageAspectFlags &dstAspectMask, uint32_t dstMipLevel,
                  uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, vk::Offset3D dstImageOffset,
                  vk::Extent3D imageExtent);

        ImageTransfer &setSourceImageLayout(vk::ImageLayout layout);

        ImageTransfer &setDestinationImageLayout(vk::ImageLayout layout);

        void execute();

    private:
        SharedCommandBuffer cmd;
        SharedImageObject src;
        SharedImageObject dst;
        vk::ImageLayout srcLayout{vk::ImageLayout::eUndefined};
        vk::ImageLayout dstLayout{vk::ImageLayout::eUndefined};
        std::vector<vk::ImageCopy> regions;
    };
}

#endif //LAVAVK_IMAGETRANSFER_H
