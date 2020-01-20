//
// Created by dorian on 08. 12. 2019..
//

#include "surface.h"
#include "lava/framework/platform/window.h"
#include "instance.h"

LavaVk::Core::Surface::Surface(const vk::SurfaceKHR &surfaceKhr) : handle(surfaceKhr)
{
}

LavaVk::Core::Surface::Surface(LavaVk::InjectionContext &context)
{
    instance = context.container.resolve<Instance>();
    handle = context.container.resolve<Window>()->createSurface(instance->getHandle());
}

LavaVk::Core::Surface::~Surface()
{
    instance->getHandle().destroy(handle);
}

vk::SurfaceKHR LavaVk::Core::Surface::getHandle() const
{
    return handle;
}

LavaVk::Core::SharedInstance LavaVk::Core::Surface::getInstance() const
{
    return instance;
}

std::type_index LavaVk::Core::Surface::getType() const
{
    return typeid(Surface);
}

