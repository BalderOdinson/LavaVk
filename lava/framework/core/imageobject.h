//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGEOBJECT_H
#define LAVAVK_IMAGEOBJECT_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "device.h"
#include "lava/framework/event-system/event.h"
#include "imageupdater.h"

namespace LavaVk::Core
{
    class ImageObject : public virtual Object
    {
    public:
        virtual vk::Image getHandle() const = 0;

        virtual SharedDevice getDevice() const = 0;

        virtual vk::ImageType getImageType() const = 0;
        virtual const vk::Extent3D &getExtent() const = 0;
        virtual vk::Format getFormat() const = 0;
        virtual vk::SampleCountFlagBits getSampleCount() const = 0;
        virtual vk::ImageUsageFlags getUsage() const = 0;
        virtual vk::ImageTiling getTiling() const = 0;
        virtual vk::ImageSubresource getSubresource() const = 0;

        virtual Event<EventHandler<>> &rebonded() = 0;
        virtual Event<EventHandler<>> &recreated() = 0;

        virtual void update(void *data, vk::DeviceSize size, uint32_t mipLevel, uint32_t baseArrayLayer,
                            uint32_t layerCount, vk::Offset3D imageOffset,
                            vk::Extent3D imageExtent, const vk::AccessFlags &currentAccess,
                            const vk::AccessFlags &newAccess,
                            const vk::PipelineStageFlags &generatingStages,
                            const vk::PipelineStageFlags &consumingStages,
                            vk::ImageLayout currentLayout, vk::ImageLayout newLayout,
                            const vk::ImageAspectFlags &aspect);

    protected:
        virtual SharedImageUpdater getUpdater() = 0;

    };

    using UniqueImageObject = std::unique_ptr<ImageObject>;
    using SharedImageObject = std::shared_ptr<ImageObject>;
    using WeakImageObject = std::weak_ptr<ImageObject>;
}


#endif //LAVAVK_IMAGEOBJECT_H
