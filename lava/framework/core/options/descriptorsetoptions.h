//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORSETOPTIONS_H
#define LAVAVK_DESCRIPTORSETOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/core/descriptorsetlayout.h"
#include "lava/framework/core/descriptorpool.h"
#include "lava/framework/vkcommon.h"

namespace LavaVk::Core
{
    class DescriptorSetOptions : public virtual Object
    {
    public:
        DescriptorSetOptions() = default;
        DescriptorSetOptions(SharedDescriptorSetLayout descriptorSetLayout,
                             SharedDescriptorPool descriptorPool,
                             BindingMap<vk::DescriptorBufferInfo> bufferInfos,
                             BindingMap<vk::BufferView> bufferViewsInfos,
                             BindingMap<vk::DescriptorImageInfo> imageInfos);

        SharedDescriptorSetLayout descriptorSetLayout;
        SharedDescriptorPool descriptorPool;
        BindingMap<vk::DescriptorBufferInfo> bufferInfos;
        BindingMap<vk::BufferView> bufferViewsInfos;
        BindingMap<vk::DescriptorImageInfo> imageInfos;

        bool equals(const Object &other) const override;
        size_t getHashCode() const override;
        std::type_index getType() const override;
    };

    using UniqueDescriptorSetOptions = std::unique_ptr<DescriptorSetOptions>;
    using SharedDescriptorSetOptions = std::shared_ptr<DescriptorSetOptions>;
    using WeakDescriptorSetOptions = std::weak_ptr<DescriptorSetOptions>;
}


#endif //LAVAVK_DESCRIPTORSETOPTIONS_H
