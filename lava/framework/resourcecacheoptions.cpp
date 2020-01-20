//
// Created by oshikuru on 27. 12. 2019..
//

#include "resourcecacheoptions.h"

LavaVk::ResourceCacheOptions::ResourceCacheOptions(uint32_t cacheDuration) : cacheDuration(cacheDuration)
{
}

std::type_index LavaVk::ResourceCacheOptions::getType() const
{
    return typeid(ResourceCacheOptions);
}
