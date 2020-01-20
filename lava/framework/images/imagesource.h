//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_IMAGESOURCE_H
#define LAVAVK_IMAGESOURCE_H

#include "lava/framework/object.h"
#include <gli/gli.hpp>

namespace LavaVk
{
    class ImageSource : public virtual Object
    {
    public:
        virtual gli::texture load() = 0;
    };

    using UniqueImageSource = std::unique_ptr<ImageSource>;
    using SharedImageSource = std::shared_ptr<ImageSource>;
    using WeakImageSource = std::weak_ptr<ImageSource>;
}


#endif //LAVAVK_IMAGESOURCE_H
