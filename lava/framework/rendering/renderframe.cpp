//
// Created by dorian on 12. 12. 2019..
//

#include "renderframe.h"

#include <utility>
#include "options/renderframeoptions.h"
#include "lava/framework/bufferpooloptions.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/core/options/commandpooloptions.h"

LavaVk::RenderFrame::RenderFrame(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Core::Device>();
    fencePool = context.container.resolve<FencePool>();
    semaphorePool = context.container.resolve<SemaphorePool>();
    auto options = context.container.option<RenderFrameOptions>(context);
    renderTarget = options->renderTarget;
    threadCount = options->threadCount;

    const std::vector<vk::BufferUsageFlagBits> supportedUsages = {vk::BufferUsageFlagBits::eUniformBuffer,
                                                                  vk::BufferUsageFlagBits::eVertexBuffer,
                                                                  vk::BufferUsageFlagBits::eIndexBuffer};

    for (auto &usage : supportedUsages)
    {
        std::vector<std::pair<SharedBufferPool, SharedBufferBlock>> usageBufferPools;
        for (size_t i = 0; i < threadCount; i++)
            usageBufferPools.emplace_back(
                    context.container.resolve<BufferPool, BufferPoolOptions>(BUFFER_POOL_BLOCK_SIZE * 1024, usage),
                    nullptr);

        auto resInsIt = bufferPools.emplace(usage, std::move(usageBufferPools));

        if (!resInsIt.second)
            throw std::runtime_error("Failed to insert buffer pool");
    }

    for (size_t i = 0; i < threadCount; i++)
    {
        descriptorPools.emplace_back();
        descriptorSets.emplace_back();
    }
}

void LavaVk::RenderFrame::updateRenderTarget(LavaVk::SharedRenderTarget target)
{
    for (auto &threadDesc : descriptorSets)
    {
        std::vector<Core::SharedDescriptorSetOptions> setsToRemove;
        for (auto &desc : threadDesc)
            for (auto &set : desc.first->imageInfos)
                for (auto &binding : set.second)
                    for (auto &img : renderTarget->getViews())
                        if (binding.second.imageView == img->getHandle())
                            setsToRemove.push_back(desc.first);

        for (auto &set : setsToRemove)
            threadDesc.erase(set);
    }

    renderTarget = std::move(target);
}

void LavaVk::RenderFrame::reset()
{
    fencePool->wait();

    fencePool->reset();

    for (auto &commandPoolsPerQueue : commandPools)
    {
        for (auto &command_pool : commandPoolsPerQueue.second)
        {
            command_pool->reset();
        }
    }

    for (auto &bufferPoolsPerUsage : bufferPools)
    {
        for (auto &buffer_pool : bufferPoolsPerUsage.second)
        {
            buffer_pool.first->reset();
            buffer_pool.second = nullptr;
        }
    }

    semaphorePool->reset();
}

std::vector<LavaVk::Core::SharedCommandPool> &
LavaVk::RenderFrame::getCommandPools(const LavaVk::Core::SharedQueue &queue,
                                     LavaVk::Core::CommandBuffer::ResetMode resetMode)
{
    auto commandPoolIt = commandPools.find({queue->getIndex().family, resetMode});
    if (commandPoolIt != commandPools.end())
        return commandPoolIt->second;
    std::vector<Core::SharedCommandPool> queueCommandPools(threadCount);
    for (size_t i = 0; i < threadCount; ++i)
        queueCommandPools[i] = Application::instance->container.resolve<Core::CommandPool, Core::CommandPoolOptions>(
                resetMode, true, queue->getIndex().family, i, std::dynamic_pointer_cast<RenderFrame>(shared_from_this()));

    auto resInsIt = commandPools.emplace(CommandPoolIndex{queue->getIndex().family, resetMode},
                                         std::move(queueCommandPools));

    if (!resInsIt.second)
        throw std::runtime_error("Failed to insert command pool");

    return resInsIt.first->second;
}


LavaVk::SharedFencePool LavaVk::RenderFrame::getFencePool() const
{
    return fencePool;
}

LavaVk::Core::SharedFence LavaVk::RenderFrame::requestFence(LavaVk::Core::FenceState desiredState)
{
    return fencePool->requestFence(desiredState);
}

LavaVk::SharedSemaphorePool LavaVk::RenderFrame::getSemaphorePool() const
{
    return semaphorePool;
}

LavaVk::Core::SharedSemaphore LavaVk::RenderFrame::requestSemaphore()
{
    return semaphorePool->requestSemaphore();

}

LavaVk::SharedRenderTarget LavaVk::RenderFrame::getRenderTarget() const
{
    return renderTarget;
}

LavaVk::Core::SharedCommandBuffer LavaVk::RenderFrame::requestCommandBuffer(const LavaVk::Core::SharedQueue &queue,
                                                                            LavaVk::Core::CommandBuffer::ResetMode resetMode,
                                                                            vk::CommandBufferLevel level,
                                                                            size_t threadIdx)
{
    return getCommandPools(queue, resetMode)[threadIdx]->requestBuffer(level);
}

LavaVk::Core::SharedDescriptorSet
LavaVk::RenderFrame::requestDescriptorSet(const LavaVk::Core::SharedDescriptorSetOptions &descriptorSetOptions,
                                          size_t threadIdx)
{
    if (descriptorPools[threadIdx].find(descriptorSetOptions->descriptorSetLayout) == descriptorPools[threadIdx].end())
    {
        descriptorPools[threadIdx][descriptorSetOptions->descriptorSetLayout] =
                Application::instance->container.resolve<Core::DescriptorPool, Core::DescriptorPoolOptions>(
                        descriptorSetOptions->descriptorSetLayout);
    }

    if (descriptorSets[threadIdx].find(descriptorSetOptions) == descriptorSets[threadIdx].end())
    {
        descriptorSetOptions->descriptorPool = descriptorPools[threadIdx][descriptorSetOptions->descriptorSetLayout];
        descriptorSets[threadIdx][descriptorSetOptions] = Application::instance->container.resolve<Core::DescriptorSet, Core::DescriptorSetOptions>(
                descriptorSetOptions);
    }

    return descriptorSets[threadIdx][descriptorSetOptions];
}

void LavaVk::RenderFrame::clearDescriptors()
{
    for (auto &descSetsPerThread : descriptorSets)
        descSetsPerThread.clear();

    for (auto &descPoolsPerThread : descriptorPools)
        for (auto &descPool : descPoolsPerThread)
            descPool.second.reset();
}

LavaVk::BufferAllocation
LavaVk::RenderFrame::allocateBuffer(vk::BufferUsageFlagBits usage, vk::DeviceSize size, size_t threadIdx)
{
    // Find a pool for this usage
    auto bufferPoolIt = bufferPools.find(usage);
    if (bufferPoolIt == bufferPools.end())
    {
        logger->error(std::string_format("No buffer pool for buffer usage %d", static_cast<uint32_t>(usage)));
        return BufferAllocation{};
    }

    auto bufferPool = bufferPoolIt->second.at(threadIdx).first;
    auto bufferBlock = bufferPoolIt->second.at(threadIdx).second;

    if (!bufferBlock)
        bufferBlock = bufferPool->requestBufferBlock(static_cast<uint32_t>(size));

    auto data = bufferBlock->allocate(static_cast<uint32_t >(size));

    // Check if the buffer block can allocate the requested size
    if (data.empty())
    {
        bufferBlock = bufferPool->requestBufferBlock(static_cast<uint32_t >(size));
        data = bufferBlock->allocate(static_cast<uint32_t >(size));
    }

    return data;
}

std::type_index LavaVk::RenderFrame::getType() const
{
    return typeid(RenderFrame);
}

LavaVk::CommandPoolIndex::CommandPoolIndex(uint32_t queueFamilyIndex, LavaVk::Core::CommandBuffer::ResetMode resetMode)
        : queueFamilyIndex(queueFamilyIndex), resetMode(resetMode)
{
}

std::type_index LavaVk::CommandPoolIndex::getType() const
{
    return typeid(CommandPoolIndex);
}

bool LavaVk::CommandPoolIndex::equals(const LavaVk::Object &other) const
{
    auto idx = dynamic_cast<const CommandPoolIndex *>(&other);
    if (idx)
        return queueFamilyIndex == idx->queueFamilyIndex && resetMode == idx->resetMode;
    return false;
}

size_t LavaVk::CommandPoolIndex::getHashCode() const
{
    return hash<uint32_t, uint32_t>()(std::make_tuple(queueFamilyIndex, static_cast<uint32_t>(resetMode)));
}
