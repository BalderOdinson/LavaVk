//
// Created by dorian on 11. 12. 2019..
//

#include "sampleroptions.h"

LavaVk::Core::SamplerOptions::SamplerOptions(const vk::Filter &magFilter, const vk::Filter &minFilter,
                                             const vk::SamplerMipmapMode &mipmapMode,
                                             const std::array<vk::SamplerAddressMode, 3> &addressMode, float mipLodBias,
                                             bool anisotropyEnable, float maxAnisotropy, bool compareEnable,
                                             const vk::CompareOp &compareOp, float minLod, float maxLod,
                                             const vk::BorderColor &borderColor, bool unnormalizedCoordinates)
        : magFilter(magFilter), minFilter(minFilter), mipmapMode(mipmapMode), addressMode(addressMode),
          mipLodBias(mipLodBias), anisotropyEnable(anisotropyEnable), maxAnisotropy(maxAnisotropy),
          compareEnable(compareEnable), compareOp(compareOp), minLod(minLod), maxLod(maxLod), borderColor(borderColor),
          unnormalizedCoordinates(unnormalizedCoordinates)
{
}

std::type_index LavaVk::Core::SamplerOptions::getType() const
{
    return typeid(SamplerOptions);
}
