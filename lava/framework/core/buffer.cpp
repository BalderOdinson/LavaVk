//
// Created by dorian on 09. 12. 2019..
//

#include "buffer.h"

#include <utility>
#include "lava/framework/platform/application.h"
#include "lava/framework/constants.h"
#include "commandpool.h"
#include "fence.h"
#include "queue.h"
#include "pipelinebarrier.h"
#include "buffertransfer.h"

LavaVk::Core::Buffer::Buffer(LavaVk::InjectionContext &context)
{
    allocator = context.container.resolve<Allocator>();
    options = *context.container.option<BufferOptions>(context);
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;
    allocationCreateInfo.usage = options.memoryUsage;
    size = options.size * options.n;
    vk::BufferCreateInfo createInfo{};
    createInfo.size = size;
    createInfo.usage = options.bufferUsage;
    createInfo.sharingMode = vk::SharingMode::eExclusive;

    auto[buffer, allocation] = allocator->getHandle().createBuffer(createInfo, allocationCreateInfo);
    handle = buffer;
    allocationHandle = allocation;
}

LavaVk::Core::Buffer::Buffer(LavaVk::Core::BufferOptions o)
        : allocator(Application::instance->container.resolve<Allocator>()), options(std::move(o))
{
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;
    allocationCreateInfo.usage = options.memoryUsage;
    size = options.size * options.n;
    vk::BufferCreateInfo createInfo{};
    createInfo.size = size;
    createInfo.usage = options.bufferUsage;
    createInfo.sharingMode = vk::SharingMode::eExclusive;

    auto[buffer, allocation] = allocator->getHandle().createBuffer(createInfo, allocationCreateInfo);
    handle = buffer;
    allocationHandle = allocation;
}

LavaVk::Core::Buffer::~Buffer()
{
    if (handle)
    {
        allocator->getHandle().destroyBuffer(handle, allocationHandle);
        handle = nullptr;
        allocationHandle = nullptr;
    }
}

vk::Buffer LavaVk::Core::Buffer::getHandle() const
{
    return handle;
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::Buffer::rebonded()
{
    return rebondedEventSubject;
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::Buffer::recreated()
{
    return recreatedChangedEventSubject;
}

vk::BufferUsageFlags LavaVk::Core::Buffer::getUsage() const
{
    return options.bufferUsage;
}

vma::Allocation LavaVk::Core::Buffer::getAllocation() const
{
    return allocationHandle;
}

LavaVk::Core::SharedAllocator LavaVk::Core::Buffer::getAllocator() const
{
    return allocator;
}

void LavaVk::Core::Buffer::recreate()
{
    allocator->getHandle().destroyBuffer(handle, allocationHandle);
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;
    allocationCreateInfo.usage = options.memoryUsage;
    size = options.size * options.n;
    vk::BufferCreateInfo createInfo{};
    createInfo.size = size;
    createInfo.usage = options.bufferUsage;
    createInfo.sharingMode = vk::SharingMode::eExclusive;

    auto[buffer, allocation] = allocator->getHandle().createBuffer(createInfo, allocationCreateInfo);
    handle = buffer;
    allocationHandle = allocation;
    recreatedChangedEventSubject.raise(*this);
}

void LavaVk::Core::Buffer::rebind()
{
    allocator->getDevice()->getHandle().destroyBuffer(handle);
    vk::BufferCreateInfo createInfo{};
    createInfo.size = options.size * options.n;
    createInfo.usage = options.bufferUsage;
    createInfo.sharingMode = vk::SharingMode::eExclusive;
    handle = allocator->getDevice()->getHandle().createBuffer(createInfo);
    rebondedEventSubject.raise(*this);
}

std::type_index LavaVk::Core::Buffer::getType() const
{
    return typeid(Buffer);
}

vk::DeviceSize LavaVk::Core::Buffer::getSize() const
{
    return size;
}

void LavaVk::Core::Buffer::getData(void *data, size_t *s) const
{
    *s = size;

    if (!data)
        return;

    auto allocationInfo = getAllocationInfo();
    auto memFlags = allocator->getHandle().getMemoryTypeProperties(allocationInfo.memoryType);
    if (memFlags & vk::MemoryPropertyFlagBits::eHostVisible)
    {
        if (allocationInfo.pMappedData)
        {
            invalidate(0, VK_WHOLE_SIZE);
            memcpy(data, allocationInfo.pMappedData, size);
        }
        else
        {
            memcpy(data, map(), size);
            unmap();
        }
    }
    else
        throw std::runtime_error("Buffer is located on GPU. To get data issue a Buffer copy command!");
}

void LavaVk::Core::Buffer::update(void *data, vk::DeviceSize size, vk::DeviceSize offset,
                                  const vk::AccessFlags &currentAccess,
                                  const vk::AccessFlags &newAccess, const vk::PipelineStageFlags &generatingStages,
                                  const vk::PipelineStageFlags &consumingStages)
{

    SharedBufferUpdater updater;
    auto allocationInfo = getAllocationInfo();
    auto memFlags = allocator->getHandle().getMemoryTypeProperties(allocationInfo.memoryType);
    if (memFlags & vk::MemoryPropertyFlagBits::eHostVisible)
        updater = Application::instance->container.resolve<BufferUpdater>(Constants::MemoryUpdater::HostId);
    else
        updater = Application::instance->container.resolve<BufferUpdater>(Constants::MemoryUpdater::DeviceId);
    updater->update(std::dynamic_pointer_cast<Buffer>(shared_from_this()), data, size, offset, currentAccess, newAccess,
                    generatingStages, consumingStages);
}
