//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_INSTANCEOPTIONS_H
#define LAVAVK_INSTANCEOPTIONS_H

#include <vector>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class InstanceOptions : public virtual Object
    {
    public:
        InstanceOptions(std::string applicationName, std::array<uint32_t, 3> version,
                        std::vector<const char *> requiredExtensions = {},
                        std::vector<const char *> requiredValidationLayers = {});

        std::type_index getType() const override;

        std::string applicationName;
        std::array<uint32_t, 3> version;
        std::vector<const char *> requiredExtensions;
        std::vector<const char *> requiredValidationLayers;
    };
}


#endif //LAVAVK_INSTANCEOPTIONS_H
