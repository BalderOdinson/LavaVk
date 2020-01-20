//
// Created by dorian on 05. 01. 2020..
//

#include "geometryrenderoptions.h"

LavaVk::GeometryRenderOptions::GeometryRenderOptions(LavaVk::AlphaMode nodeMode) : nodeMode(nodeMode)
{
}

std::type_index LavaVk::GeometryRenderOptions::getType() const
{
    return typeid(GeometryRenderOptions);
}
