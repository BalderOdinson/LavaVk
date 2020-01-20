//
// Created by dorian on 12. 12. 2019..
//

#include "fencepool.h"
#include "platform/application.h"

LavaVk::FencePool::FencePool(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Core::Device>();
}

LavaVk::FencePool::~FencePool()
{
    wait();
}

LavaVk::Core::SharedFence LavaVk::FencePool::requestFence(Core::FenceState desiredState)
{
    // Check if there is an available fence
    if (activeFenceCount < fences.size())
        return fences.at(activeFenceCount++);

    fences.push_back(Application::instance->container.resolve<Core::Fence, Core::FenceOptions>(desiredState));
    activeFenceCount++;
    return fences.back();
}

bool LavaVk::FencePool::wait(uint32_t timeout) const
{
    if (activeFenceCount < 1 || fences.empty())
        return true;

    return Core::Fence::waitForFences(fences.begin(), fences.begin() + activeFenceCount, timeout);
}

void LavaVk::FencePool::reset()
{
    if (activeFenceCount < 1 || fences.empty())
        return;

    Core::Fence::resetFences(fences.begin(), fences.begin() + activeFenceCount);
    activeFenceCount = 0;
}

std::type_index LavaVk::FencePool::getType() const
{
    return typeid(FencePool);
}
