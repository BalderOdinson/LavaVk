//
// Created by dorian on 09. 12. 2019..
//

#include "fence.h"
#include "options/fenceoptions.h"

LavaVk::Core::Fence::Fence(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<FenceOptions>(context);
    handle = device->getHandle().createFence(vk::FenceCreateInfo().setFlags(
            options->signaled == FenceState::Signaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlags{}));
}

bool LavaVk::Core::Fence::wait(uint64_t timeout) const
{
    return device->getHandle().waitForFences({handle}, true, timeout) == vk::Result::eSuccess;
}

void LavaVk::Core::Fence::reset() const
{
    device->getHandle().resetFences({handle});
}

LavaVk::Core::Fence::~Fence()
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::Fence LavaVk::Core::Fence::getHandle() const
{
    return handle;
}

std::type_index LavaVk::Core::Fence::getType() const
{
    return typeid(Fence);
}
