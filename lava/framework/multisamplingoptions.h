//
// Created by oshikuru on 27. 12. 2019..
//

#ifndef LAVAVK_MULTISAMPLINGOPTIONS_H
#define LAVAVK_MULTISAMPLINGOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "object.h"

namespace LavaVk
{
    class MultisamplingOptions : public virtual Object
    {
    public:
        MultisamplingOptions() = default;

        explicit MultisamplingOptions(vk::SampleCountFlagBits samples);

        vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;

        std::type_index getType() const override;
    };

    using UniqueMultisamplingOptions = std::unique_ptr<MultisamplingOptions>;
    using SharedMultisamplingOptions = std::shared_ptr<MultisamplingOptions>;
    using WeakMultisamplingOptions = std::weak_ptr<MultisamplingOptions>;
}


#endif //LAVAVK_MULTISAMPLINGOPTIONS_H
