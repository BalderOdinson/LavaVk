//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGEVIEWOPTIONS_H
#define LAVAVK_IMAGEVIEWOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/core/imageobject.h"

namespace LavaVk::Core
{
    class ImageViewOptions : public virtual Object
    {
    public:
        ImageViewOptions() = default;
        ImageViewOptions(SharedImageObject image, vk::ImageViewType viewType, vk::Format format = vk::Format::eUndefined);

        SharedImageObject image{};
        vk::ImageViewType viewType{};
        vk::Format format{vk::Format::eUndefined};

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_IMAGEVIEWOPTIONS_H
