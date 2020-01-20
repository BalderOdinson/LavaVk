//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_IMAGELOADER_H
#define LAVAVK_IMAGELOADER_H

#include "lava/framework/object.h"
#include <gli/gli.hpp>

namespace LavaVk
{
    class ImageLoader : public virtual Object
    {
    public:
        virtual gli::texture load(std::string_view filename) = 0;
    };

    using UniqueImageLoader = std::unique_ptr<ImageLoader>;
    using SharedImageLoader = std::shared_ptr<ImageLoader>;
    using WeakImageLoader = std::weak_ptr<ImageLoader>;
}


#endif //LAVAVK_IMAGELOADER_H
