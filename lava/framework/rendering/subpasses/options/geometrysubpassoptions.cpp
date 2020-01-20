//
// Created by dorian on 17. 12. 2019..
//

#include "geometrysubpassoptions.h"

#include <utility>

LavaVk::GeometrySubpassOptions::GeometrySubpassOptions(std::vector<SharedComponentRenderer> renderers) : renderers(std::move(renderers))
{
}

std::type_index LavaVk::GeometrySubpassOptions::getType() const
{
    return typeid(GeometrySubpassOptions);
}
