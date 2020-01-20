//
// Created by dorian on 10. 12. 2019..
//

#include "buffertransfer.h"
#include "commandbuffer.h"

#include <utility>

LavaVk::Core::BufferTransfer::BufferTransfer(LavaVk::Core::SharedCommandBuffer cmd, LavaVk::Core::SharedBuffer src,
                                             LavaVk::Core::SharedBuffer dst) : cmd(std::move(cmd)), src(std::move(src)),
                                                                               dst(std::move(dst))
{
}

LavaVk::Core::BufferTransfer &
LavaVk::Core::BufferTransfer::addRegion(vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize size)
{
    regions.emplace_back(srcOffset, dstOffset, size);
    return *this;
}

void LavaVk::Core::BufferTransfer::execute()
{
    cmd->getHandle().copyBuffer(src->getHandle(), dst->getHandle(), regions);
}
