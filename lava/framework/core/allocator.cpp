//
// Created by dorian on 08. 12. 2019..
//

#include "allocator.h"
#include "options/allocatoroptions.h"

LavaVk::Core::Allocator::Allocator(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<AllocatorOptions>(context);
    vma::AllocatorCreateInfo info{};
#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
    auto functions = bindVulkanFunctions(VULKAN_HPP_DEFAULT_DISPATCHER);
    info.pVulkanFunctions = &functions;
#endif
    info.physicalDevice = device->getPhysicalDevice();
    info.device = device->getHandle();
    if(device->isEnabled(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
        info.flags = vma::AllocatorCreateFlagBits::eKhrDedicatedAllocation;
    info.frameInUseCount = options->frameInUseCount;

    handle = vma::createAllocator(info);
}

LavaVk::Core::Allocator::~Allocator()
{
    if(handle)
    {
        handle.destroy();
        handle = nullptr;
    }
}

vma::Allocator LavaVk::Core::Allocator::getHandle() const
{
    return handle;
}

LavaVk::Core::SharedDevice LavaVk::Core::Allocator::getDevice() const
{
    return device;
}

vma::VulkanFunctions LavaVk::Core::Allocator::bindVulkanFunctions(const vk::DispatchLoaderDynamic &dl)
{
    vma::VulkanFunctions functions;
    functions.vkAllocateMemory = dl.vkAllocateMemory;
    functions.vkBindBufferMemory = dl.vkBindBufferMemory;
    functions.vkBindImageMemory = dl.vkBindImageMemory;
    functions.vkCmdCopyBuffer = dl.vkCmdCopyBuffer;
    functions.vkCreateBuffer = dl.vkCreateBuffer;
    functions.vkCreateImage = dl.vkCreateImage;
    functions.vkDestroyBuffer = dl.vkDestroyBuffer;
    functions.vkDestroyImage = dl.vkDestroyImage;
    functions.vkFlushMappedMemoryRanges = dl.vkFlushMappedMemoryRanges;
    functions.vkFreeMemory = dl.vkFreeMemory;
    functions.vkGetBufferMemoryRequirements = dl.vkGetBufferMemoryRequirements;
    functions.vkGetBufferMemoryRequirements2KHR = dl.vkGetBufferMemoryRequirements2KHR;
    functions.vkGetImageMemoryRequirements = dl.vkGetImageMemoryRequirements;
    functions.vkGetImageMemoryRequirements2KHR = dl.vkGetImageMemoryRequirements2KHR;
    functions.vkGetPhysicalDeviceMemoryProperties = dl.vkGetPhysicalDeviceMemoryProperties;
    functions.vkGetPhysicalDeviceProperties = dl.vkGetPhysicalDeviceProperties;
    functions.vkInvalidateMappedMemoryRanges = dl.vkInvalidateMappedMemoryRanges;
    functions.vkMapMemory = dl.vkMapMemory;
    functions.vkUnmapMemory = dl.vkUnmapMemory;
    return functions;
}

std::type_index LavaVk::Core::Allocator::getType() const
{
    return typeid(Allocator);
}
