//
// Created by dorian on 04. 01. 2020..
//

#include "basematerialsource.h"

LavaVk::BaseMaterialSource::BaseMaterialSource(LavaVk::SharedMaterial material) : material(std::move(material))
{
}

LavaVk::SharedMaterial LavaVk::BaseMaterialSource::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    for (auto &texture : material->textures)
        texture.second->load(cmd);
    return material;
}

std::type_index LavaVk::BaseMaterialSource::getType() const
{
    return typeid(BaseMaterialSource);
}
