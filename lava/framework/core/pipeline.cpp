//
// Created by dorian on 13. 12. 2019..
//

#include "pipeline.h"

LavaVk::Core::Pipeline::Pipeline(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
}

LavaVk::Core::Pipeline::~Pipeline()
{
    if(handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::Pipeline LavaVk::Core::Pipeline::getHandle() const
{
    return handle;
}

const LavaVk::PipelineState &LavaVk::Core::Pipeline::getState() const
{
    return state;
}
