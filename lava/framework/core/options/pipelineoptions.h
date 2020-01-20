//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_PIPELINEOPTIONS_H
#define LAVAVK_PIPELINEOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/rendering/pipelinestate.h"

namespace LavaVk::Core
{
    class PipelineOptions : public virtual Object
    {
    public:
        explicit PipelineOptions(PipelineState& state, vk::PipelineCache cache = {});

        PipelineState& state;
        vk::PipelineCache cache;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniquePipelineOptions = std::unique_ptr<PipelineOptions>;
    using SharedPipelineOptions = std::shared_ptr<PipelineOptions>;
    using WeakPipelineOptions = std::weak_ptr<PipelineOptions>;
}


#endif //LAVAVK_PIPELINEOPTIONS_H
