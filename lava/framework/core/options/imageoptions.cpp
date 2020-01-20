//
// Created by dorian on 10. 12. 2019..
//

#include "imageoptions.h"

LavaVk::Core::ImageOptions::ImageOptions(vma::MemoryUsage memoryUsage, bool isMapped, vk::Extent3D extent,
                                         vk::Format format,
                                         const vk::ImageUsageFlags &imageUsage, bool canGetLost,
                                         const vk::MemoryPropertyFlags &requiredFlags,
                                         const vk::MemoryPropertyFlags &preferredFlags,
                                         vk::SampleCountFlagBits sampleCount, uint32_t mipLevels, uint32_t arrayLayers,
                                         vk::ImageTiling imageTiling, vk::ImageCreateFlags flags) : AllocationOption(memoryUsage, isMapped,
                                                                                         canGetLost, requiredFlags,
                                                                                         preferredFlags),
                                                                        extent(extent), format(format),
                                                                        imageUsage(imageUsage),
                                                                        sampleCount(sampleCount), mipLevels(mipLevels),
                                                                        arrayLayers(arrayLayers),
                                                                        imageTiling(imageTiling),
                                                                        flags(flags)
{
}

std::type_index LavaVk::Core::ImageOptions::getType() const
{
    return typeid(ImageOptions);
}
