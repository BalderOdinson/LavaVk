//
// Created by dorian on 04. 01. 2020..
//

#ifndef LAVAVK_SPECULARMATERIAL_H
#define LAVAVK_SPECULARMATERIAL_H

#include "material.h"

namespace LavaVk
{
    class SpecularMaterial : public Material
    {
    public:
        explicit SpecularMaterial(std::string_view name);

        std::type_index getType() const override;

        /// Diffuse color of the material
        glm::vec4 diffuse{1.0f, 1.0f, 1.0f, 1.0f};
        /// Ambient color of the material
        glm::vec4 ambient{0.1f, 0.1f, 0.1f, 1.0f};
        /// Specular color of the material. Forth component is material shininess
        glm::vec4 specular{1.0f, 1.0f, 1.0f, 60.0f};

        std::vector<uint8_t> data() const override;
    };
}


#endif //LAVAVK_SPECULARMATERIAL_H
