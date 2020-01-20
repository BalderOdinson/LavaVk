//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGE2BUFFERTRANSFER_H
#define LAVAVK_IMAGE2BUFFERTRANSFER_H

#include <vulkan/vulkan.hpp>
#include "imageobject.h"
#include "buffer.h"

namespace LavaVk::Core
{
    class CommandBuffer;

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    class Image2BufferTransfer
    {
    public:
        Image2BufferTransfer(SharedCommandBuffer cmd, SharedImageObject src, SharedBuffer dst);

        Image2BufferTransfer &
        addRegion(vk::DeviceSize bufferOffset, vk::DeviceSize bufferRowLength, vk::DeviceSize bufferImageHeight,
                  const vk::ImageAspectFlags &aspectMask, uint32_t mipLevel, uint32_t baseArrayLayer,
                  uint32_t layerCount, vk::Offset3D imageOffset, vk::Extent3D imageExtent);

        Image2BufferTransfer &
        addRegion(vk::DeviceSize bufferOffset, const vk::ImageAspectFlags &aspectMask, uint32_t mipLevel,
                  uint32_t baseArrayLayer, uint32_t layerCount, vk::Offset3D imageOffset, vk::Extent3D imageExtent);

        Image2BufferTransfer &setSourceImageLayout(vk::ImageLayout layout);

        void execute();

    private:
        SharedCommandBuffer cmd;
        SharedImageObject src;
        SharedBuffer dst;
        vk::ImageLayout srcLayout{vk::ImageLayout::eUndefined};
        std::vector<vk::BufferImageCopy> regions;

    };
}


#endif //LAVAVK_IMAGE2BUFFERTRANSFER_H
