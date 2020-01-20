//
// Created by dorian on 09. 12. 2019..
//

#include "semaphore.h"

LavaVk::Core::Semaphore::Semaphore(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    handle = device->getHandle().createSemaphore({});
}

LavaVk::Core::Semaphore::~Semaphore()
{
    if(handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::Semaphore LavaVk::Core::Semaphore::getHandle() const
{
    return handle;
}

std::type_index LavaVk::Core::Semaphore::getType() const
{
    return typeid(Semaphore);
}
