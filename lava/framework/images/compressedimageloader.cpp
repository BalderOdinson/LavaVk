//
// Created by oshikuru on 28. 12. 2019..
//

#include "compressedimageloader.h"

gli::texture LavaVk::CompressedImageLoader::load(std::string_view filename)
{
    return gli::load(filename.data());
}

std::type_index LavaVk::CompressedImageLoader::getType() const
{
    return typeid(CompressedImageLoader);
}
