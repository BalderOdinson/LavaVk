//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BUFFERTRANSFER_H
#define LAVAVK_BUFFERTRANSFER_H

#include <vulkan/vulkan.hpp>
#include "buffer.h"

namespace LavaVk::Core
{
    class CommandBuffer;

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    class BufferTransfer
    {
    public:
        BufferTransfer(SharedCommandBuffer cmd, SharedBuffer src, SharedBuffer dst);

        BufferTransfer &addRegion(vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize size);

        void execute();

    private:
        SharedCommandBuffer cmd;
        SharedBuffer src;
        SharedBuffer dst;
        std::vector<vk::BufferCopy> regions;

    };
}


#endif //LAVAVK_BUFFERTRANSFER_H
