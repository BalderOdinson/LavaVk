//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_COMPRESSEDIMAGELOADER_H
#define LAVAVK_COMPRESSEDIMAGELOADER_H


#include "imageloader.h"

namespace LavaVk
{
    class CompressedImageLoader : public ImageLoader
    {
    public:
        gli::texture load(std::string_view filename) override;
        std::type_index getType() const override;
    };
}


#endif //LAVAVK_COMPRESSEDIMAGELOADER_H
