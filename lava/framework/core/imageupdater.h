//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGEUPDATER_H
#define LAVAVK_IMAGEUPDATER_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{

    class ImageObject;

    using UniqueImageObject = std::unique_ptr<ImageObject>;
    using SharedImageObject = std::shared_ptr<ImageObject>;
    using WeakImageObject = std::weak_ptr<ImageObject>;

    class ImageUpdater : public virtual Object
    {
    public:
        /// Updates image with given data.
        /// \param dst Destination image.
        /// \param data Source data.
        /// \param size Size of data.
        /// \param mipLevel Number (index) of mipmap level to be updated
        /// \param baseArrayLayer Number of the first array layer to be updated
        /// \param layerCount Number of array layers that will be updated
        /// \param imageOffset Initial offset (in texels) of image's sub region that should be updated.
        /// \param imageExtent The size (dimensions) of an image.
        /// \param aspect Image's aspect (color, depth or stencil)
        virtual void
        update(SharedImageObject dst, void *data, vk::DeviceSize size, uint32_t mipLevel, uint32_t baseArrayLayer,
               uint32_t layerCount, vk::Offset3D imageOffset,
               vk::Extent3D imageExtent, const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
               const vk::PipelineStageFlags &generatingStages, const vk::PipelineStageFlags &consumingStages,
               vk::ImageLayout currentLayout, vk::ImageLayout newLayout, const vk::ImageAspectFlags &aspect) = 0;
    };

    using UniqueImageUpdater = std::unique_ptr<ImageUpdater>;
    using SharedImageUpdater = std::shared_ptr<ImageUpdater>;
    using WeakImageUpdater = std::weak_ptr<ImageUpdater>;
}


#endif //LAVAVK_IMAGEUPDATER_H
