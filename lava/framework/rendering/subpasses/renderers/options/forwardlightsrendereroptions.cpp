//
// Created by dorian on 06. 01. 2020..
//

#include "forwardlightsrendereroptions.h"

LavaVk::ForwardLightsRendererOptions::ForwardLightsRendererOptions(LavaVk::SharedComponentRenderer baseRenderer,
                                                                   uint32_t set, uint32_t binding,
                                                                   std::string_view shaderNameExtension,
                                                                   const vk::ShaderStageFlags& stages) : baseRenderer(
        std::move(baseRenderer)), set(set), binding(binding), shaderNameExtension(shaderNameExtension), stages(stages)
{
}

std::type_index LavaVk::ForwardLightsRendererOptions::getType() const
{
    return typeid(ForwardLightsRendererOptions);
}
