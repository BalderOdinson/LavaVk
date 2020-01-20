//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORPOOLOPTIONS_H
#define LAVAVK_DESCRIPTORPOOLOPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/core/descriptorsetlayout.h"

namespace LavaVk::Core
{
    class DescriptorPoolOptions : public virtual Object
    {
    public:
        DescriptorPoolOptions() = default;
        explicit DescriptorPoolOptions(SharedDescriptorSetLayout descriptorSetLayout,
                                       uint32_t poolSize = MAX_POOL_SIZE);

        SharedDescriptorSetLayout descriptorSetLayout;
        uint32_t poolSize = MAX_POOL_SIZE;

        std::type_index getType() const override;

        static constexpr uint32_t MAX_POOL_SIZE = 16;
    };

    using UniqueDescriptorPoolOptions = std::unique_ptr<DescriptorPoolOptions>;
    using SharedDescriptorPoolOptions = std::shared_ptr<DescriptorPoolOptions>;
    using WeakDescriptorPoolOptions = std::weak_ptr<DescriptorPoolOptions>;
}


#endif //LAVAVK_DESCRIPTORPOOLOPTIONS_H
