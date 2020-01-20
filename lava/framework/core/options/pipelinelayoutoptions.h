//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_PIPELINELAYOUTOPTIONS_H
#define LAVAVK_PIPELINELAYOUTOPTIONS_H

#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/core/shadermodule.h"

namespace LavaVk::Core
{
    class PipelineLayoutOptions : public virtual Object
    {
    public:
        PipelineLayoutOptions() = default;
        explicit PipelineLayoutOptions(std::vector<SharedShaderModule> shaderModules);

        std::vector<SharedShaderModule> shaderModules;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniquePipelineLayoutOptions = std::unique_ptr<PipelineLayoutOptions>;
    using SharedPipelineLayoutOptions = std::shared_ptr<PipelineLayoutOptions>;
    using WeakPipelineLayoutOptions = std::weak_ptr<PipelineLayoutOptions>;
}

#endif //LAVAVK_PIPELINELAYOUTOPTIONS_H
