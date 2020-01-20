//
// Created by dorian on 08. 12. 2019..
//

#include "queue.h"
#include "device.h"

LavaVk::Core::Queue::Queue(LavaVk::InjectionContext &context)
{
    auto options = context.container.option<QueueOptions>(context);
    auto device = context.container.resolve<Device>();
    auto surface = context.container.resolve<Surface>();
    index = options->index;
    properties = options->properties;
    handle = device->getHandle().getQueue(index.family, index.index);
    canPresent = device->getPhysicalDevice().getSurfaceSupportKHR(index.family, surface->getHandle());
}

vk::Queue LavaVk::Core::Queue::getHandle() const
{
    return handle;
}

const LavaVk::Core::QueueIndex &LavaVk::Core::Queue::getIndex() const
{
    return index;
}

vk::QueueFamilyProperties LavaVk::Core::Queue::getProperties() const
{
    return properties;
}

bool LavaVk::Core::Queue::supportPresent() const
{
    return canPresent;
}

LavaVk::Core::QueueSubmitRequest LavaVk::Core::Queue::createSubmitRequest()
{
    return LavaVk::Core::QueueSubmitRequest(handle);
}

LavaVk::Core::QueuePresentRequest LavaVk::Core::Queue::createPresentRequest()
{
    return LavaVk::Core::QueuePresentRequest(handle);
}

void LavaVk::Core::Queue::waitIdle() const
{
    handle.waitIdle();
}

std::type_index LavaVk::Core::Queue::getType() const
{
    return typeid(Queue);
}
