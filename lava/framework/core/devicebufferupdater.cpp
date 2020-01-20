//
// Created by dorian on 10. 12. 2019..
//

#include "lava/framework/core/options/devicememoryupdateoptions.h"
#include "devicebufferupdater.h"
#include "lava/framework/constants.h"
#include "options/stagingbufferoptions.h"
#include "pipelinebarrier.h"
#include "buffertransfer.h"

LavaVk::Core::DeviceBufferUpdater::DeviceBufferUpdater(LavaVk::InjectionContext &context)
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

void
LavaVk::Core::DeviceBufferUpdater::update(LavaVk::Core::SharedBuffer buffer, void *data, size_t size, size_t offset,
                                          const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
                                          const vk::PipelineStageFlags &generatingStages,
                                          const vk::PipelineStageFlags &consumingStages)
{
    SharedBuffer stagingBuffer = make<Buffer>(StagingBufferOptions(size, 1));

    bufferUpdater->update(stagingBuffer, data, size, 0, {}, {}, {}, {});

    if (!commandBuffer)
        commandBuffer = pool->requestBuffer(vk::CommandBufferLevel::ePrimary);
    {
        BeginToken recorder;
        if (commandBuffer->getState() != CommandBuffer::State::Recording)
            recorder = commandBuffer->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        PipelineBarrier(commandBuffer).addBufferMemoryBarrier(buffer->getHandle(),
                                                              currentAccess,
                                                              vk::AccessFlagBits::eTransferWrite,
                                                              consumingStages,
                                                              vk::PipelineStageFlagBits::eTransfer,
                                                              VK_QUEUE_FAMILY_IGNORED,
                                                              VK_QUEUE_FAMILY_IGNORED).execute();
        BufferTransfer(commandBuffer, stagingBuffer, buffer).addRegion(0, offset, size).execute();
        PipelineBarrier(commandBuffer).addBufferMemoryBarrier(buffer->getHandle(),
                                                              vk::AccessFlagBits::eTransferWrite,
                                                              newAccess,
                                                              vk::PipelineStageFlagBits::eTransfer,
                                                              consumingStages,
                                                              VK_QUEUE_FAMILY_IGNORED,
                                                              VK_QUEUE_FAMILY_IGNORED).execute();
    }

    if (transferQueue)
    {
        fence->reset();
        transferQueue->createSubmitRequest().setFence(fence->getHandle())
                .submitInfo()
                .addCommandBuffer(commandBuffer->getHandle()).add()
                .submit();
        fence->wait();
    }
}

std::type_index LavaVk::Core::DeviceBufferUpdater::getType() const
{
    return typeid(DeviceBufferUpdater);
}
