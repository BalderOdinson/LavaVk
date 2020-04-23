//
// Created by dorian on 16. 12. 2019..
//

#ifndef LAVAVK_SUBPASSOPTIONS_H
#define LAVAVK_SUBPASSOPTIONS_H

#include <unordered_map>
#include "lava/framework/object.h"
#include "lava/framework/core/options/shadermoduleoptions.h"
#include "dynamicstateconfigurator.h"

namespace LavaVk
{
    class SubpassOptions : public virtual Object
    {
    public:
        SubpassOptions();
        explicit SubpassOptions(SharedDynamicStateConfigurator configurator);

        SharedDynamicStateConfigurator configurator{};

        std::type_index getType() const override;
    };
}

#endif //LAVAVK_SUBPASSOPTIONS_H
