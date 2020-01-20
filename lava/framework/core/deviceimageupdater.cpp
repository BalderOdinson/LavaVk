//
// Created by dorian on 10. 12. 2019..
//

#include "deviceimageupdater.h"
#include "lava/framework/constants.h"
#include "buffer.h"
#include "options/stagingbufferoptions.h"
#include "buffer2imagetransfer.h"
#include "fence.h"
#include "hostbufferupdater.h"
#include "pipelinebarrier.h"
#include "imageobject.h"
#include "lava/framework/core/options/devicememoryupdateoptions.h"

LavaVk::Core::DeviceImageUpdater::DeviceImageUpdater(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    allocator = context.container.resolve<Allocator>();
    auto options = context.container.option<DeviceMemoryUpdateOptions>(context);
    bufferUpdater = context.container.resolve<BufferUpdater>(Constants::MemoryUpdater::HostId);
    transferQueue = options->queue;
    pool = options->pool;
    fence = options->fence;
    commandBuffer = options->commandBuffer;
}

void LavaVk::Core::DeviceImageUpdater::update(LavaVk::Core::SharedImageObject dst, void *data, vk::DeviceSize size,
                                              uint32_t mipLevel,
                                              uint32_t baseArrayLayer, uint32_t layerCount, vk::Offset3D imageOffset,
                                              vk::Extent3D imageExtent, const vk::AccessFlags &currentAccess,
                                              const vk::AccessFlags &newAccess,
                                              const vk::PipelineStageFlags &generatingStages,
                                              const vk::PipelineStageFlags &consumingStages,
                                              vk::ImageLayout currentLayout, vk::ImageLayout newLayout,
                                              const vk::ImageAspectFlags &aspect)
{
    SharedBuffer stagingBuffer = make<Buffer>(StagingBufferOptions(size, 1));

    bufferUpdater->update(stagingBuffer, data, size, 0, {}, {}, {}, {});

    if (!commandBuffer)
        commandBuffer = pool->requestBuffer(vk::CommandBufferLevel::ePrimary);
    {
        BeginToken recorder;
        if (commandBuffer->getState() != CommandBuffer::State::Recording)
            recorder = commandBuffer->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        PipelineBarrier(commandBuffer).addImageMemoryBarrier(dst->getHandle(),
                                                             currentAccess,
                                                             vk::AccessFlagBits::eTransferWrite,
                                                             generatingStages,
                                                             vk::PipelineStageFlagBits::eTransfer,
                                                             currentLayout,
                                                             vk::ImageLayout::eTransferDstOptimal,
                                                             VK_QUEUE_FAMILY_IGNORED,
                                                             VK_QUEUE_FAMILY_IGNORED,
                                                             aspect).execute();
        Buffer2ImageTransfer(commandBuffer, stagingBuffer, dst).addRegion(0, aspect, mipLevel, baseArrayLayer,
                                                                          layerCount,
                                                                          imageOffset,
                                                                          imageExtent).setDestinationImageLayout(
                vk::ImageLayout::eTransferDstOptimal).execute();
        PipelineBarrier(commandBuffer).addImageMemoryBarrier(dst->getHandle(),
                                                             vk::AccessFlagBits::eTransferWrite,
                                                             newAccess,
                                                             vk::PipelineStageFlagBits::eTransfer,
                                                             consumingStages,
                                                             vk::ImageLayout::eTransferDstOptimal,
                                                             newLayout,
                                                             VK_QUEUE_FAMILY_IGNORED,
                                                             VK_QUEUE_FAMILY_IGNORED,
                                                             aspect).execute();
    }

    if(transferQueue)
    {
        fence->reset();
        transferQueue->createSubmitRequest().setFence(fence->getHandle())
                .submitInfo()
                .addCommandBuffer(commandBuffer->getHandle()).add()
                .submit();
        fence->wait();
    }
}

std::type_index LavaVk::Core::DeviceImageUpdater::getType() const
{
    return typeid(DeviceImageUpdater);
}
