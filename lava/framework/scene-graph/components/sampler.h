//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_SCENESAMPLER_H
#define LAVAVK_SCENESAMPLER_H

#include "lava/framework/scene-graph/component.h"
#include "lava/framework/core/sampler.h"
#include "lava/framework/core/options/sampleroptions.h"
#include "lava/framework/images/mipmapsoptions.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk
{
    class Sampler : public Component
    {
    public:
        explicit Sampler(uint32_t mipmaps = 1, std::string_view name = "");
        explicit Sampler(Core::SharedSamplerOptions samplerOptions, std::string_view name = "");
        Sampler(const SharedMipmapsOptions &lodOptions, uint32_t mipmaps, std::string_view name = "");
        Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter, const vk::SamplerMipmapMode &mipmapMode,
                const std::array<vk::SamplerAddressMode, 3> &addressMode, float maxAnisotropy, uint32_t mipmaps = 1,
                std::string_view name = "");
        Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter, const vk::SamplerMipmapMode &mipmapMode,
                const std::array<vk::SamplerAddressMode, 3> &addressMode, const vk::CompareOp &compareOp,
                float maxAnisotropy, uint32_t mipmaps = 1, std::string_view name = "");
        Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter,
                const vk::SamplerMipmapMode &mipmapMode,
                const std::array<vk::SamplerAddressMode, 3> &addressMode, float mipLodBias, float maxAnisotropy,
                const vk::CompareOp &compareOp, float minLod, float maxLod, const vk::BorderColor &borderColor,
                bool unnormalizedCoordinates = false, std::string_view name = "");
        Sampler(Sampler &&other) = default;

        Core::SharedSampler getVkSampler() const;
        Core::SharedSamplerOptions getOptions() const;
        void setLod(uint32_t mipmaps, const SharedMipmapsOptions& lodOptions);

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

        std::type_index getType() const override;

    private:
        Core::SharedSampler vkSampler;
        Core::SharedSamplerOptions options;
    };

    using UniqueSampler = std::unique_ptr<Sampler>;
    using SharedSampler = std::shared_ptr<Sampler>;
    using WeakSampler = std::weak_ptr<Sampler>;
}

#endif //LAVAVK_SCENESAMPLER_H
