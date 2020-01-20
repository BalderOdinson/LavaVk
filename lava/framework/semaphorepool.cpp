//
// Created by dorian on 12. 12. 2019..
//

#include "semaphorepool.h"
#include "platform/application.h"

LavaVk::SemaphorePool::SemaphorePool(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Core::Device>();
}

LavaVk::Core::SharedSemaphore LavaVk::SemaphorePool::requestSemaphore()
{
    // Check if there is an available semaphore
    if (activeSemaphoreCount < semaphores.size())
        return semaphores.at(activeSemaphoreCount++);

    semaphores.push_back(Application::instance->container.resolve<Core::Semaphore>());
    activeSemaphoreCount++;
    return semaphores.back();
}

void LavaVk::SemaphorePool::reset()
{
    activeSemaphoreCount = 0;
}

uint32_t LavaVk::SemaphorePool::getActiveSemaphoreCount() const
{
    return activeSemaphoreCount;
}

std::type_index LavaVk::SemaphorePool::getType() const
{
    return typeid(SemaphorePool);
}
