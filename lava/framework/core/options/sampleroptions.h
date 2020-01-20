//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_SAMPLEROPTIONS_H
#define LAVAVK_SAMPLEROPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class SamplerOptions : public virtual Object
    {
    public:
        SamplerOptions() = default;
        SamplerOptions(const vk::Filter &magFilter, const vk::Filter &minFilter,
                       const vk::SamplerMipmapMode &mipmapMode,
                       const std::array<vk::SamplerAddressMode, 3> &addressMode, float mipLodBias,
                       bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
                       const vk::CompareOp &compareOp, float minLod, float maxLod,
                       const vk::BorderColor &borderColor, bool unnormalizedCoordinates);

        vk::Filter magFilter;
        vk::Filter minFilter;
        vk::SamplerMipmapMode mipmapMode;
        std::array<vk::SamplerAddressMode, 3> addressMode;
        float mipLodBias;
        bool anisotropyEnable;
        float maxAnisotropy;
        bool compareEnable;
        vk::CompareOp compareOp;
        float minLod;
        float maxLod;
        vk::BorderColor borderColor;
        bool unnormalizedCoordinates;

        std::type_index getType() const override;
    };

    using UniqueSamplerOptions = std::unique_ptr<SamplerOptions>;
    using SharedSamplerOptions = std::shared_ptr<SamplerOptions>;
    using WeakSamplerOptions = std::weak_ptr<SamplerOptions>;
}



#endif //LAVAVK_SAMPLEROPTIONS_H
