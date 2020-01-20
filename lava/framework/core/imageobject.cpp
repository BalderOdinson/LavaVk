//
// Created by dorian on 10. 12. 2019..
//

#include "imageobject.h"

void LavaVk::Core::ImageObject::update(void *data, vk::DeviceSize size, uint32_t mipLevel, uint32_t baseArrayLayer,
                                       uint32_t layerCount, vk::Offset3D imageOffset, vk::Extent3D imageExtent,
                                       const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
                                       const vk::PipelineStageFlags &generatingStages,
                                       const vk::PipelineStageFlags &consumingStages, vk::ImageLayout currentLayout,
                                       vk::ImageLayout newLayout, const vk::ImageAspectFlags &aspect)
{
    getUpdater()->update(std::dynamic_pointer_cast<ImageObject>(shared_from_this()), data, size, mipLevel,
                         baseArrayLayer, layerCount, imageOffset, imageExtent, currentAccess, newAccess,
                         generatingStages, consumingStages, currentLayout, newLayout, aspect);
}
