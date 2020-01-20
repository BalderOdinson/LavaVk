//
// Created by dorian on 06. 01. 2020..
//

#ifndef LAVAVK_FORWARDLIGHTSRENDEREROPTIONS_H
#define LAVAVK_FORWARDLIGHTSRENDEREROPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/rendering/subpasses/componentrenderer.h"

namespace LavaVk
{
    class ForwardLightsRendererOptions : public virtual Object
    {
    public:
        ForwardLightsRendererOptions() = default;
        ForwardLightsRendererOptions(SharedComponentRenderer baseRenderer, uint32_t set, uint32_t binding,
                                     std::string_view shaderNameExtension,
                                     const vk::ShaderStageFlags& stages = vk::ShaderStageFlagBits::eFragment);

        SharedComponentRenderer baseRenderer;
        uint32_t set{0};
        uint32_t binding{0};
        std::string shaderNameExtension;
        vk::ShaderStageFlags stages{vk::ShaderStageFlagBits::eFragment};

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_FORWARDLIGHTSRENDEREROPTIONS_H
