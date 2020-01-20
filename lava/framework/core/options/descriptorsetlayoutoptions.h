//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORSETLAYOUTOPTIONS_H
#define LAVAVK_DESCRIPTORSETLAYOUTOPTIONS_H

#include <vector>
#include "lava/framework/object.h"
#include "shaderresource.h"

namespace LavaVk::Core
{
    class DescriptorSetLayoutOptions : public virtual Object
    {
    public:
        DescriptorSetLayoutOptions() = default;
        explicit DescriptorSetLayoutOptions(std::vector<ShaderResource> setResources);

        std::vector<ShaderResource> setResources;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniqueDescriptorSetLayoutOptions = std::unique_ptr<DescriptorSetLayoutOptions>;
    using SharedDescriptorSetLayoutOptions = std::shared_ptr<DescriptorSetLayoutOptions>;
    using WeakDescriptorSetLayoutOptions = std::weak_ptr<DescriptorSetLayoutOptions>;
}


#endif //LAVAVK_DESCRIPTORSETLAYOUTOPTIONS_H
