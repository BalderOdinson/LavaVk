//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BUFFER2IMAGETRANSFER_H
#define LAVAVK_BUFFER2IMAGETRANSFER_H

#include <vulkan/vulkan.hpp>
#include "imageobject.h"
#include "buffer.h"

namespace LavaVk::Core
{
    class CommandBuffer;

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    class Buffer2ImageTransfer
    {
    public:
        Buffer2ImageTransfer(SharedCommandBuffer cmd, SharedBuffer src, SharedImageObject dst);

        Buffer2ImageTransfer &
        addRegion(vk::DeviceSize bufferOffset, vk::DeviceSize bufferRowLength, vk::DeviceSize bufferImageHeight,
                  const vk::ImageAspectFlags &aspectMask, uint32_t mipLevel, uint32_t baseArrayLayer,
                  uint32_t layerCount, vk::Offset3D imageOffset, vk::Extent3D imageExtent);

        Buffer2ImageTransfer &
        addRegion(vk::DeviceSize bufferOffset, const vk::ImageAspectFlags &aspectMask, uint32_t mipLevel,
                  uint32_t baseArrayLayer, uint32_t layerCount, vk::Offset3D imageOffset, vk::Extent3D imageExtent);

        Buffer2ImageTransfer &setDestinationImageLayout(vk::ImageLayout layout);

        void execute();

    private:
        SharedCommandBuffer cmd;
        SharedBuffer src;
        SharedImageObject dst;
        vk::ImageLayout dstLayout{vk::ImageLayout::eUndefined};
        std::vector<vk::BufferImageCopy> regions;
    };
}


#endif //LAVAVK_BUFFER2IMAGETRANSFER_H
