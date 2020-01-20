//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_APPLICATIONOPTION_H
#define LAVAVK_APPLICATIONOPTION_H

#include <array>
#include <vector>
#include "lava/framework/object.h"

namespace LavaVk
{
    class ApplicationOption : public virtual Object
    {
    public:
        std::string appName;
        std::array<uint32_t, 3> version;
        bool enableDebug;
        std::vector<const char*> additionalRequiredExtensions;
        std::vector<const char*> additionalRequiredLayers;
        std::type_index getType() const override;
    };
}


#endif //LAVAVK_APPLICATIONOPTION_H
