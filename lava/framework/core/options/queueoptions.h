//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_QUEUEOPTIONS_H
#define LAVAVK_QUEUEOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class QueueIndex : public virtual Object
    {
    public:
        QueueIndex(uint32_t family, uint32_t index);

        uint32_t family;
        uint32_t index;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    class QueueOptions : public virtual Object
    {
    public:
        QueueOptions() = default;
        QueueOptions(QueueIndex index, vk::QueueFamilyProperties properties);

        QueueIndex index{0, 0};
        vk::QueueFamilyProperties properties;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_QUEUEOPTIONS_H
