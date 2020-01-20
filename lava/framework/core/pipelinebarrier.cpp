//
// Created by dorian on 10. 12. 2019..
//

#include "pipelinebarrier.h"

#include <utility>

LavaVk::Core::PipelineBarrier::PipelineBarrier(LavaVk::Core::SharedCommandBuffer commandBuffer) : commandBuffer(
        std::move(commandBuffer))
{
}

LavaVk::Core::PipelineBarrier::PipelineBarrier(LavaVk::InjectionContext &context) : PipelineBarrier(
        context.container.option<CommandBuffer>(context))
{
}

LavaVk::Core::PipelineBarrier &LavaVk::Core::PipelineBarrier::setDependencyFlags(const vk::DependencyFlags &flags)
{
    dependencyFlags = flags;
    return *this;

}

LavaVk::Core::PipelineBarrier &
LavaVk::Core::PipelineBarrier::addGlobalMemoryBarrier(const vk::AccessFlags &srcAccessFlags,
                                                      const vk::AccessFlags &dstAccessFlags,
                                                      const vk::PipelineStageFlags &genStages,
                                                      const vk::PipelineStageFlags &consStages)
{
    generatingStages |= genStages;
    consumingStages |= consStages;
    vk::MemoryBarrier barrier{};
    barrier.srcAccessMask = srcAccessFlags;
    barrier.dstAccessMask = dstAccessFlags;
    memoryBarriers.push_back(barrier);
    return *this;

}

LavaVk::Core::PipelineBarrier &
LavaVk::Core::PipelineBarrier::addBufferMemoryBarrier(const vk::Buffer &buffer, const vk::AccessFlags &srcAccessFlags,
                                                      const vk::AccessFlags &dstAccessFlags,
                                                      const vk::PipelineStageFlags &genStages,
                                                      const vk::PipelineStageFlags &consStages,
                                                      uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex)
{
    generatingStages |= genStages;
    consumingStages |= consStages;
    vk::BufferMemoryBarrier barrier{};
    barrier.srcAccessMask = srcAccessFlags;
    barrier.dstAccessMask = dstAccessFlags;
    barrier.buffer = buffer;
    barrier.offset = 0;
    barrier.size = VK_WHOLE_SIZE;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    if (currentQueueFamilyIndex != newQueueFamilyIndex)
    {
        barrier.srcQueueFamilyIndex = currentQueueFamilyIndex;
        barrier.dstQueueFamilyIndex = newQueueFamilyIndex;
    }
    bufferMemoryBarriers.push_back(barrier);
    return *this;

}

LavaVk::Core::PipelineBarrier &
LavaVk::Core::PipelineBarrier::addBufferMemoryBarrier(const vk::Buffer &buffer, const vk::AccessFlags &srcAccessFlags,
                                                      const vk::AccessFlags &dstAccessFlags,
                                                      const vk::PipelineStageFlags &genStages,
                                                      const vk::PipelineStageFlags &consStages,
                                                      uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex,
                                                      vk::DeviceSize offset, vk::DeviceSize size)
{
    generatingStages |= genStages;
    consumingStages |= consStages;
    vk::BufferMemoryBarrier barrier{};
    barrier.srcAccessMask = srcAccessFlags;
    barrier.dstAccessMask = dstAccessFlags;
    barrier.buffer = buffer;
    barrier.offset = offset;
    barrier.size = size;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    if (currentQueueFamilyIndex != newQueueFamilyIndex)
    {
        barrier.srcQueueFamilyIndex = currentQueueFamilyIndex;
        barrier.dstQueueFamilyIndex = newQueueFamilyIndex;
    }
    bufferMemoryBarriers.push_back(barrier);
    return *this;

}

LavaVk::Core::PipelineBarrier &
LavaVk::Core::PipelineBarrier::addImageMemoryBarrier(const vk::Image &image, const vk::AccessFlags &srcAccessFlags,
                                                     const vk::AccessFlags &dstAccessFlags,
                                                     const vk::PipelineStageFlags &genStages,
                                                     const vk::PipelineStageFlags &consStages,
                                                     const vk::ImageLayout &currentImageLayout,
                                                     const vk::ImageLayout &newImageLayout,
                                                     uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex,
                                                     const vk::ImageAspectFlags &aspect)
{
    generatingStages |= genStages;
    consumingStages |= consStages;
    vk::ImageMemoryBarrier barrier{};
    barrier.image = image;
    barrier.srcAccessMask = srcAccessFlags;
    barrier.dstAccessMask = dstAccessFlags;
    barrier.oldLayout = currentImageLayout;
    barrier.newLayout = newImageLayout;
    barrier.subresourceRange.aspectMask = aspect;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = VK_REMAINING_MIP_LEVELS;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    if (currentQueueFamilyIndex != newQueueFamilyIndex)
    {
        barrier.srcQueueFamilyIndex = currentQueueFamilyIndex;
        barrier.dstQueueFamilyIndex = newQueueFamilyIndex;
    }
    imageMemoryBarriers.push_back(barrier);
    return *this;

}

void LavaVk::Core::PipelineBarrier::execute() const
{
    commandBuffer->getHandle().pipelineBarrier(generatingStages, consumingStages, dependencyFlags, memoryBarriers,
                                               bufferMemoryBarriers, imageMemoryBarriers);
}
