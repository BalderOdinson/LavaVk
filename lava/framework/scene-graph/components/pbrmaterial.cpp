//
// Created by dorian on 15. 12. 2019..
//

#include "pbrmaterial.h"

LavaVk::PBRMaterial::PBRMaterial(std::string_view name) : Material(name)
{
}

std::type_index LavaVk::PBRMaterial::getType() const
{
    return typeid(PBRMaterial);
}

std::vector<uint8_t> LavaVk::PBRMaterial::data() const
{
    std::vector<uint8_t> result(sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(float) + sizeof(float));
    auto data = result.data();
    memcpy(data, &emissive, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    memcpy(data, &baseColorFactor, sizeof(glm::vec4));
    data += sizeof(glm::vec4);
    memcpy(data, &metallicFactor, sizeof(float));
    data += sizeof(float);
    memcpy(data, &roughnessFactor, sizeof(float));
    return result;
}
