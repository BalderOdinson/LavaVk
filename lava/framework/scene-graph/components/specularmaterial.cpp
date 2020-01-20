//
// Created by dorian on 04. 01. 2020..
//

#include "specularmaterial.h"

LavaVk::SpecularMaterial::SpecularMaterial(std::string_view name) : Material(name)
{
}

std::type_index LavaVk::SpecularMaterial::getType() const
{
    return Material::getType();
}

std::vector<uint8_t> LavaVk::SpecularMaterial::data() const
{
    std::vector<uint8_t> result(
            sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec4));
    auto data = result.data();
    memcpy(data, &emissive, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    memcpy(data, &diffuse, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    memcpy(data, &ambient, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    memcpy(data, &specular, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    return result;
}
