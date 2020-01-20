//
// Created by dorian on 12. 12. 2019..
//

#include "bufferpool.h"
#include "lava/framework/platform/application.h"
#include "bufferpooloptions.h"

#include <utility>

LavaVk::BufferAllocation::BufferAllocation(LavaVk::Core::SharedBuffer buffer, vk::DeviceSize size,
                                           vk::DeviceSize offset) : buffer(std::move(buffer)), baseOffset(offset),
                                                                    size(size)
{
}

void LavaVk::BufferAllocation::update(const std::vector<uint8_t> &data, vk::DeviceSize offset,
                                      const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
                                      const vk::PipelineStageFlags &generatingStages,
                                      const vk::PipelineStageFlags &consumingStages)
{
    if (offset + data.size() <= size)
        buffer->update(const_cast<uint8_t *>(data.data()), data.size(), offset, currentAccess, newAccess,
                       generatingStages,
                       consumingStages);
    else
        Application::instance->container.resolve<Logger>()->error("Update data exceeds buffer allocation size");
}

bool LavaVk::BufferAllocation::empty() const
{
    return size == 0 || buffer;
}

vk::DeviceSize LavaVk::BufferAllocation::getSize() const
{
    return size;
}

vk::DeviceSize LavaVk::BufferAllocation::getOffset() const
{
    return baseOffset;
}

LavaVk::Core::SharedBuffer &LavaVk::BufferAllocation::getBuffer()
{
    return buffer;
}

LavaVk::BufferBlock::BufferBlock(vk::DeviceSize size, const vk::BufferUsageFlags &usage, vma::MemoryUsage memoryUsage)
{
    buffer = Application::instance->container.resolve<Core::Buffer, Core::BufferOptions>(1u, size, usage, memoryUsage,
                                                                                         false);

    if (usage & vk::BufferUsageFlagBits::eUniformBuffer)
        alignment = Application::instance->container.resolve<Core::Device>()->getProperties().limits.minUniformBufferOffsetAlignment;
    else if (usage & vk::BufferUsageFlagBits::eStorageBuffer)
        alignment = Application::instance->container.resolve<Core::Device>()->getProperties().limits.minStorageBufferOffsetAlignment;
    else if (usage & vk::BufferUsageFlagBits::eUniformTexelBuffer ||
             usage & vk::BufferUsageFlagBits::eStorageTexelBuffer)
        alignment = Application::instance->container.resolve<Core::Device>()->getProperties().limits.minTexelBufferOffsetAlignment;
    else if (usage & vk::BufferUsageFlagBits::eIndexBuffer || usage & vk::BufferUsageFlagBits::eVertexBuffer ||
             usage & vk::BufferUsageFlagBits::eIndirectBuffer)
        // Used to calculate the offset, required when allocating memory (its value should be power of 2)
        alignment = 16;
    else
        throw std::runtime_error("Usage not recognised");
}

LavaVk::BufferAllocation LavaVk::BufferBlock::allocate(uint32_t allocateSize)
{
    auto alignedOffset = (offset + alignment - 1) & ~(alignment - 1);

    if (alignedOffset + allocateSize > buffer->getSize())
    {
        // No more space available from the underlying buffer, return empty allocation
        return BufferAllocation{};
    }

    // Move the current offset and return an allocation
    offset = alignedOffset + allocateSize;
    return BufferAllocation{buffer, allocateSize, alignedOffset};
}

vk::DeviceSize LavaVk::BufferBlock::getSize() const
{
    return buffer->getSize();
}

void LavaVk::BufferBlock::reset()
{
    offset = 0;
}

std::type_index LavaVk::BufferBlock::getType() const
{
    return typeid(BufferBlock);
}

LavaVk::BufferPool::BufferPool(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Core::Device>();
    logger = context.container.resolve<Logger>();
    auto options = context.container.option<BufferPoolOptions>(context);
    blockSize = options->blockSize;
    usage = options->usage;
    memoryUsage = options->memoryUsage;
}

LavaVk::SharedBufferBlock LavaVk::BufferPool::requestBufferBlock(vk::DeviceSize minimumSize)
{
    // Find the first block in the range of the inactive blocks
    // which size is greater than the minimum size
    auto it = std::upper_bound(bufferBlocks.begin() + activeBufferBlockCount, bufferBlocks.end(), minimumSize,
                               [](const vk::DeviceSize &a, const SharedBufferBlock &b) -> bool
                               { return a < b->getSize(); });

    if (it != bufferBlocks.end())
    {
        // Recycle inactive block
        activeBufferBlockCount++;
        return *it;
    }

    logger->debug(
            std::string_format("Building #%ld buffer block (%d)", bufferBlocks.size(), static_cast<uint32_t>(usage)));

    // Create a new block, store and return it
    bufferBlocks.emplace_back(std::make_shared<BufferBlock>(std::max(blockSize, minimumSize), usage, memoryUsage));

    return bufferBlocks[activeBufferBlockCount++];
}

void LavaVk::BufferPool::reset()
{
    for (auto &bufferBlock : bufferBlocks)
    {
        bufferBlock->reset();
    }

    activeBufferBlockCount = 0;
}

std::type_index LavaVk::BufferPool::getType() const
{
    return typeid(BufferPool);
}
