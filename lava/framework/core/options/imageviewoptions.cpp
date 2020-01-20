//
// Created by dorian on 10. 12. 2019..
//

#include "imageviewoptions.h"

#include <utility>

LavaVk::Core::ImageViewOptions::ImageViewOptions(SharedImageObject img, vk::ImageViewType viewType, vk::Format format)
        : image(std::move(img)), viewType(viewType), format(format)
{
}

std::type_index LavaVk::Core::ImageViewOptions::getType() const
{
    return typeid(ImageViewOptions);
}
