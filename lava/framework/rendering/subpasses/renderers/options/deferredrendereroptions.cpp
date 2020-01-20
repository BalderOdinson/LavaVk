//
// Created by oshikuru on 20. 01. 2020..
//

#include "deferredrendereroptions.h"

#include <utility>

LavaVk::DeferredRendererOptions::DeferredRendererOptions(LavaVk::SharedComponentRenderer baseRenderer, std::string_view shaderNameExtension)
        : baseRenderer(std::move(baseRenderer)), shaderNameExtension(shaderNameExtension)
{}

std::type_index LavaVk::DeferredRendererOptions::getType() const
{
    return typeid(DeferredRendererOptions);
}
