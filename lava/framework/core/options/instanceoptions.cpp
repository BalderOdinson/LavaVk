//
// Created by dorian on 08. 12. 2019..
//

#include "instanceoptions.h"

#include <utility>

std::type_index LavaVk::Core::InstanceOptions::getType() const
{
    return typeid(InstanceOptions);
}

LavaVk::Core::InstanceOptions::InstanceOptions(std::string applicationName,
                                               std::array<uint32_t, 3> version,
                                               std::vector<const char *> requiredExtensions,
                                               std::vector<const char *> requiredValidationLayers)
        : applicationName(std::move(applicationName)), version(version), requiredExtensions(std::move(requiredExtensions)),
          requiredValidationLayers(std::move(requiredValidationLayers))
{
}
