//
// Created by dorian on 14. 12. 2019..
//

#include "sampler.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/dependency-injection/dicontainer.h"

#include <utility>

std::type_index LavaVk::Sampler::getType() const
{
    return typeid(Sampler);
}

LavaVk::Sampler::Sampler(uint32_t mipmaps, std::string_view name) : Component(name)
{
    auto lodOptions = Application::instance->container.option<MipmapsOptions>();
    auto lodBias = (static_cast<float>(LevelOfDetail::VeryHigh) - static_cast<float>(lodOptions->lod)) * 0.1f;
    auto anisotropy = Application::instance->container.resolve<Core::Device>()->getFeatures().samplerAnisotropy;
    auto maxAnisotropy = Application::instance->container.resolve<Core::Device>()->getProperties().limits.maxSamplerAnisotropy;
    options = make<LavaVk::Core::SamplerOptions>(
            vk::Filter::eLinear, vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
            std::array<vk::SamplerAddressMode, 3>{
                    vk::SamplerAddressMode::eRepeat,
                    vk::SamplerAddressMode::eRepeat,
                    vk::SamplerAddressMode::eRepeat}, lodBias, anisotropy, maxAnisotropy,
            false, vk::CompareOp::eAlways, 0.f,
            mipmaps,
            vk::BorderColor::eIntOpaqueBlack, false);
}

LavaVk::Sampler::Sampler(const SharedMipmapsOptions &lodOptions, uint32_t mipmaps, std::string_view name)
        : Component(name)
{
    auto lodBias = (static_cast<float>(LevelOfDetail::VeryHigh) - static_cast<float>(lodOptions->lod)) * 0.1f;
    auto anisotropy = Application::instance->container.resolve<Core::Device>()->getFeatures().samplerAnisotropy;
    auto maxAnisotropy = Application::instance->container.resolve<Core::Device>()->getProperties().limits.maxSamplerAnisotropy;
    options = make<LavaVk::Core::SamplerOptions>(
            vk::Filter::eLinear, vk::Filter::eLinear,
            vk::SamplerMipmapMode::eLinear,
            std::array<vk::SamplerAddressMode, 3>{
                    vk::SamplerAddressMode::eRepeat,
                    vk::SamplerAddressMode::eRepeat,
                    vk::SamplerAddressMode::eRepeat}, lodBias, anisotropy, maxAnisotropy,
            false, vk::CompareOp::eAlways, 0.f,
            mipmaps,
            vk::BorderColor::eIntOpaqueBlack, false);
}

LavaVk::Sampler::Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter,
                         const vk::SamplerMipmapMode &mipmapMode,
                         const std::array<vk::SamplerAddressMode, 3> &addressMode, float maxAnisotropy,
                         uint32_t mipmaps,
                         std::string_view name) : Component(name)
{
    auto lodOptions = Application::instance->container.option<MipmapsOptions>();
    auto lodBias = (static_cast<float>(LevelOfDetail::VeryHigh) - static_cast<float>(lodOptions->lod)) * 0.1f;
    auto anisotropy = Application::instance->container.resolve<Core::Device>()->getFeatures().samplerAnisotropy;
    maxAnisotropy = std::clamp(maxAnisotropy, 1.f,
                               Application::instance->container.resolve<Core::Device>()->getProperties().limits.maxSamplerAnisotropy);
    options = make<LavaVk::Core::SamplerOptions>(
            magFilter, minFilter, mipmapMode,
            addressMode, lodBias, anisotropy, maxAnisotropy,
            false, vk::CompareOp::eAlways, 0.f,
            mipmaps, vk::BorderColor::eIntOpaqueBlack, false);
}

LavaVk::Sampler::Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter,
                         const vk::SamplerMipmapMode &mipmapMode,
                         const std::array<vk::SamplerAddressMode, 3> &addressMode, const vk::CompareOp &compareOp,
                         float maxAnisotropy, uint32_t mipmaps, std::string_view name) : Component(name)
{
    auto lodOptions = Application::instance->container.option<MipmapsOptions>();
    auto lodBias = (static_cast<float>(LevelOfDetail::VeryHigh) - static_cast<float>(lodOptions->lod)) * 0.1f;
    auto anisotropy = Application::instance->container.resolve<Core::Device>()->getFeatures().samplerAnisotropy;
    maxAnisotropy = std::clamp(maxAnisotropy, 1.f,
                               Application::instance->container.resolve<Core::Device>()->getProperties().limits.maxSamplerAnisotropy);
    options = make<LavaVk::Core::SamplerOptions>(magFilter, minFilter, mipmapMode, addressMode, lodBias, anisotropy,
                                                 maxAnisotropy, true, compareOp, 0.f, mipmaps,
                                                 vk::BorderColor::eIntOpaqueBlack, false);
}

LavaVk::Sampler::Sampler(const vk::Filter &magFilter, const vk::Filter &minFilter,
                         const vk::SamplerMipmapMode &mipmapMode,
                         const std::array<vk::SamplerAddressMode, 3> &addressMode, float mipLodBias,
                         float maxAnisotropy, const vk::CompareOp &compareOp, float minLod, float maxLod,
                         const vk::BorderColor &borderColor, bool unnormalizedCoordinates, std::string_view name)
        : Component(name)
{
    auto anisotropy = Application::instance->container.resolve<Core::Device>()->getFeatures().samplerAnisotropy;
    maxAnisotropy = std::clamp(maxAnisotropy, 1.f,
                               Application::instance->container.resolve<Core::Device>()->getProperties().limits.maxSamplerAnisotropy);
    options = make<LavaVk::Core::SamplerOptions>(magFilter, minFilter, mipmapMode, addressMode, mipLodBias, anisotropy,
                                                 maxAnisotropy, true, compareOp, minLod, maxLod, borderColor,
                                                 unnormalizedCoordinates);
}

LavaVk::Sampler::Sampler(LavaVk::Core::SharedSamplerOptions samplerOptions, std::string_view name) : Component(name),
                                                                                                       options(std::move(
                                                                                                               samplerOptions))
{
}

LavaVk::Core::SharedSampler LavaVk::Sampler::getVkSampler() const
{
    return vkSampler;
}

LavaVk::Core::SharedSamplerOptions LavaVk::Sampler::getOptions() const
{
    return options;
}

bool LavaVk::Sampler::isValid()
{
    return bool(vkSampler);
}

void LavaVk::Sampler::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    vkSampler = Application::instance->container.resolve<LavaVk::Core::Sampler, LavaVk::Core::SamplerOptions>(options);
}

void LavaVk::Sampler::setLod(uint32_t mipmaps, const LavaVk::SharedMipmapsOptions &lodOptions)
{
    auto lodBias = (static_cast<float>(LevelOfDetail::VeryHigh) - static_cast<float>(lodOptions->lod)) * 0.1f;
    options->mipLodBias = lodBias;
    options->maxLod = mipmaps;
}
