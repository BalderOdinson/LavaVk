//
// Created by dorian on 09. 12. 2019..
//

#include "allocatoroptions.h"

std::type_index LavaVk::Core::AllocatorOptions::getType() const
{
    return typeid(AllocatorOptions);
}

LavaVk::Core::AllocatorOptions::AllocatorOptions(uint32_t frameInUseCount) : frameInUseCount(frameInUseCount)
{
}
