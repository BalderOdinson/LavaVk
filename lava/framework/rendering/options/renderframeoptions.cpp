//
// Created by dorian on 12. 12. 2019..
//

#include "renderframeoptions.h"

#include <utility>

LavaVk::RenderFrameOptions::RenderFrameOptions(LavaVk::SharedRenderTarget renderTarget, size_t threadCount)
        : renderTarget(std::move(renderTarget)), threadCount(threadCount)
{
}

std::type_index LavaVk::RenderFrameOptions::getType() const
{
    return typeid(RenderFrameOptions);
}
