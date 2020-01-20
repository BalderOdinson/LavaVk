//
// Created by dorian on 09. 12. 2019..
//

#include "commandpool.h"
#include "options/commandpooloptions.h"
#include "lava/framework/rendering/renderframe.h"

LavaVk::Core::CommandPool::CommandPool(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<CommandPoolOptions>(context);
    vk::CommandPoolCreateInfo info{};
    resetMode = options->resetMode;
    releaseOnReset = options->releaseOnReset;
    queueFamilyIndex = options->queueFamilyIndex;
    threadIndex = options->threadIndex;
    frame = options->frame;
    info.queueFamilyIndex = options->queueFamilyIndex;
    info.flags = options->resetMode == CommandBuffer::ResetMode::ResetIndividually
                 ? vk::CommandPoolCreateFlagBits::eResetCommandBuffer
                 : vk::CommandPoolCreateFlagBits::eTransient;

    handle = device->getHandle().createCommandPool(info);
}

LavaVk::Core::SharedCommandBuffer LavaVk::Core::CommandPool::requestBuffer(vk::CommandBufferLevel level)
{
    if (level == vk::CommandBufferLevel::ePrimary)
    {
        if (activePrimaryBuffersCount < allocatedPrimaryBuffers.size())
            return allocatedPrimaryBuffers[activePrimaryBuffersCount++];

        allocatedPrimaryBuffers.emplace_back(
                new CommandBuffer(std::dynamic_pointer_cast<CommandPool>(shared_from_this()), level));

        activePrimaryBuffersCount++;
        return allocatedPrimaryBuffers.back();
    }
    else
    {
        if (activeSecondaryBuffersCount < allocatedSecondaryBuffers.size())
            return allocatedSecondaryBuffers[activeSecondaryBuffersCount++];

        allocatedSecondaryBuffers.emplace_back(new CommandBuffer(std::dynamic_pointer_cast<CommandPool>(shared_from_this()), level));
        activeSecondaryBuffersCount++;
        return allocatedSecondaryBuffers.back();
    }
}

vk::CommandPool LavaVk::Core::CommandPool::getHandle() const
{
    return handle;
}

LavaVk::Core::CommandPool::~CommandPool()
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

void LavaVk::Core::CommandPool::reset()
{
    switch (resetMode)
    {
        case CommandBuffer::ResetMode::ResetIndividually:
            resetCommandBuffers();
            break;
        case CommandBuffer::ResetMode::ResetPool:
            device->getHandle().resetCommandPool(handle, {});
            resetCommandBuffers();
            break;
    }
}

void LavaVk::Core::CommandPool::resetCommandBuffers()
{
    for (auto &cmd : allocatedPrimaryBuffers)
        cmd->reset(resetMode);

    activePrimaryBuffersCount = 0;

    for (auto &cmd : allocatedSecondaryBuffers)
        cmd->reset(resetMode);

    activeSecondaryBuffersCount = 0;
}

std::type_index LavaVk::Core::CommandPool::getType() const
{
    return typeid(CommandPool);
}

uint32_t LavaVk::Core::CommandPool::getQueueFamilyIndex() const
{
    return queueFamilyIndex;
}

uint32_t LavaVk::Core::CommandPool::getThreadIndex() const
{
    return threadIndex;
}

LavaVk::SharedRenderFrame LavaVk::Core::CommandPool::getFrame() const
{
    return frame.expired() ? nullptr : frame.lock();
}

LavaVk::Core::CommandBuffer::ResetMode LavaVk::Core::CommandPool::getResetMode() const
{
    return resetMode;
}
