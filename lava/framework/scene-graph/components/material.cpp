//
// Created by dorian on 14. 12. 2019..
//

#include "material.h"

LavaVk::Material::Material(std::string_view name) : Component(name)
{
}

std::type_index LavaVk::Material::getType() const
{
    return typeid(Material);
}

bool LavaVk::Material::isValid()
{
    return std::all_of(textures.begin(), textures.end(), [](const auto &texture)
    { return texture.second && texture.second->isValid(); });
}

void LavaVk::Material::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    for (auto &texture : textures)
        if(!texture.second->isValid())
            texture.second->load(commandBuffer);
}
