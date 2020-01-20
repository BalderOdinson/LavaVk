//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_DEVICEOPTIONS_H
#define LAVAVK_DEVICEOPTIONS_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class DeviceOptions : public virtual Object
    {
    public:
        DeviceOptions() = default;

        std::vector<const char*> extensions;
        vk::PhysicalDeviceFeatures requestedFeatures;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_DEVICEOPTIONS_H
