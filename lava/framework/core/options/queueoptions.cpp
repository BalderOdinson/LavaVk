//
// Created by dorian on 08. 12. 2019..
//

#include "queueoptions.h"

#include <utility>

LavaVk::Core::QueueIndex::QueueIndex(uint32_t family, uint32_t index) : family(family), index(index)
{
}

std::type_index LavaVk::Core::QueueIndex::getType() const
{
    return typeid(QueueIndex);
}

size_t LavaVk::Core::QueueIndex::getHashCode() const
{
    uint32_t shift = sizeof(size_t) / 2;
    size_t hash = (static_cast<size_t>(family) << shift) + index;
    return hash;
}

std::type_index LavaVk::Core::QueueOptions::getType() const
{
    return typeid(QueueOptions);
}

LavaVk::Core::QueueOptions::QueueOptions(LavaVk::Core::QueueIndex index, vk::QueueFamilyProperties properties) : index(
        std::move(
                index)), properties(std::move(properties))
{
}
