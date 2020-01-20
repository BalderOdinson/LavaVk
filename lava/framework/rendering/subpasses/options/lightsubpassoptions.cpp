//
// Created by oshikuru on 20. 01. 2020..
//

#include "lightsubpassoptions.h"

#include <utility>

LavaVk::LightSubpassOptions::LightSubpassOptions(LavaVk::SharedLightRenderer renderer) : renderer(std::move(renderer))
{}

std::type_index LavaVk::LightSubpassOptions::getType() const
{
    return typeid(LightSubpassOptions);
}
