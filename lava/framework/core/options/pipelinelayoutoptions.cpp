//
// Created by dorian on 13. 12. 2019..
//

#include "pipelinelayoutoptions.h"

#include <utility>

LavaVk::Core::PipelineLayoutOptions::PipelineLayoutOptions(std::vector<SharedShaderModule> shaderModules) : shaderModules(
        std::move(shaderModules))
{
}

std::type_index LavaVk::Core::PipelineLayoutOptions::getType() const
{
    return typeid(PipelineLayoutOptions);
}

size_t LavaVk::Core::PipelineLayoutOptions::getHashCode() const
{
    size_t result = 0;
    for (auto &module : shaderModules)
        glm::detail::hash_combine(result, module->getId());
    return result;
}
