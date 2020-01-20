//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_BITMAPIMAGELOADER_H
#define LAVAVK_BITMAPIMAGELOADER_H

#include "imageloader.h"

namespace LavaVk
{
    class BitmapImageLoader : public ImageLoader
    {
    public:
        gli::texture load(std::string_view filename) override;
        std::type_index getType() const override;
    };
}


#endif //LAVAVK_BITMAPIMAGELOADER_H
