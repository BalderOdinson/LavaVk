//
// Created by dorian on 16. 12. 2019..
//

#include "subpassoptions.h"

#include <utility>

std::type_index LavaVk::SubpassOptions::getType() const
{
    return typeid(SubpassOptions);
}

LavaVk::SubpassOptions::SubpassOptions() : configurator(new DynamicStateConfigurator())
{
}

LavaVk::SubpassOptions::SubpassOptions(LavaVk::SharedDynamicStateConfigurator configurator) : configurator(std::move(
        configurator))
{}
