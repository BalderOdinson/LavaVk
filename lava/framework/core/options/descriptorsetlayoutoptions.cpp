//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorsetlayoutoptions.h"
#include "lava/third-party/glm.h"

LavaVk::Core::DescriptorSetLayoutOptions::DescriptorSetLayoutOptions(std::vector<ShaderResource> setResources)
        : setResources(std::move(setResources))
{
}

std::type_index LavaVk::Core::DescriptorSetLayoutOptions::getType() const
{
    return typeid(DescriptorSetLayoutOptions);
}

size_t LavaVk::Core::DescriptorSetLayoutOptions::getHashCode() const
{
    size_t result = 0;
    for (const auto & setResource : setResources)
    {
        size_t midResult = 0;

        if (setResource.type == ShaderResourceType::Input ||
            setResource.type == ShaderResourceType::Output ||
            setResource.type == ShaderResourceType::PushConstant ||
            setResource.type == ShaderResourceType::SpecializationConstant)
        {
            glm::detail::hash_combine(result, midResult);
            continue;
        }

        glm::detail::hash_combine(midResult, setResource.set);
        glm::detail::hash_combine(midResult, setResource.binding);
        glm::detail::hash_combine(midResult,
                                  static_cast<std::underlying_type<ShaderResourceType>::type>(setResource.type));

        glm::detail::hash_combine(result, midResult);
    }
    return result;
}
