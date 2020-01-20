//
// Created by dorian on 31. 12. 2019..
//

#include "mipmapsoptions.h"

LavaVk::MipmapsOptions::MipmapsOptions(uint32_t maxLod, LevelOfDetail lod) : maxLod(maxLod), lod(lod)
{
}

std::type_index LavaVk::MipmapsOptions::getType() const
{
    return typeid(MipmapsOptions);
}
