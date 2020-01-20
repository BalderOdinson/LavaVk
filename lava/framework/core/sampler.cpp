//
// Created by dorian on 11. 12. 2019..
//

#include "sampler.h"
#include "options/sampleroptions.h"
#include "lava/framework/platform/application.h"

LavaVk::Core::Sampler::Sampler(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<SamplerOptions>(context);

    vk::SamplerCreateInfo info{};
    info.magFilter = options->magFilter;
    info.minFilter = options->minFilter;
    info.mipmapMode = options->mipmapMode;
    info.addressModeU = options->addressMode[0];
    info.addressModeV = options->addressMode[1];
    info.addressModeW = options->addressMode[2];
    info.mipLodBias = options->mipLodBias;
    info.anisotropyEnable = options->anisotropyEnable;
    info.maxAnisotropy = options->maxAnisotropy;
    info.compareEnable = options->compareEnable;
    info.compareOp = options->compareOp;
    info.minLod = options->minLod;
    info.maxLod = options->maxLod;
    info.borderColor = options->borderColor;
    info.unnormalizedCoordinates = options->unnormalizedCoordinates;

    handle = device->getHandle().createSampler(info);
}


LavaVk::Core::Sampler::Sampler(const LavaVk::Core::SamplerOptions &options)
{
    device = Application::instance->container.resolve<Device>();
    vk::SamplerCreateInfo info{};
    info.magFilter = options.magFilter;
    info.minFilter = options.minFilter;
    info.mipmapMode = options.mipmapMode;
    info.addressModeU = options.addressMode[0];
    info.addressModeV = options.addressMode[1];
    info.addressModeW = options.addressMode[2];
    info.mipLodBias = options.mipLodBias;
    info.anisotropyEnable = options.anisotropyEnable;
    info.maxAnisotropy = options.maxAnisotropy;
    info.compareEnable = options.compareEnable;
    info.compareOp = options.compareOp;
    info.minLod = options.minLod;
    info.maxLod = options.maxLod;
    info.borderColor = options.borderColor;
    info.unnormalizedCoordinates = options.unnormalizedCoordinates;

    handle = device->getHandle().createSampler(info);
}

LavaVk::Core::Sampler::~Sampler()
{
    if(handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::Sampler LavaVk::Core::Sampler::getHandle() const
{
    return handle;
}

std::type_index LavaVk::Core::Sampler::getType() const
{
    return typeid(Sampler);
}
