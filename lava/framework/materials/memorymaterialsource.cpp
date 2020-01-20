//
// Created by dorian on 03. 01. 2020..
//

#include "memorymaterialsource.h"

LavaVk::MemoryMaterialSource::MemoryMaterialSource(LavaVk::SharedMaterial material) : material(std::move(material))
{
}

LavaVk::SharedMaterial LavaVk::MemoryMaterialSource::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    for (auto &texture : material->textures)
        texture.second->load(cmd);
    return material;
}

std::type_index LavaVk::MemoryMaterialSource::getType() const
{
    return typeid(MemoryMaterialSource);
}
