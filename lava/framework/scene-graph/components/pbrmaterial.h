//
// Created by dorian on 15. 12. 2019..
//

#ifndef LAVAVK_PBRMATERIAL_H
#define LAVAVK_PBRMATERIAL_H

#include "lava/third-party/glm.h"
#include "material.h"

namespace LavaVk
{
    class PBRMaterial : public Material
    {
    public:
        explicit PBRMaterial(std::string_view name);

        std::type_index getType() const override;

        glm::vec4 baseColorFactor{0.0f, 0.0f, 0.0f, 0.0f};
        float metallicFactor{0.0f};
        float roughnessFactor{0.0f};

        std::vector<uint8_t> data() const override;
    };

    using UniquePBRMaterial = std::unique_ptr<PBRMaterial>;
    using SharedPBRMaterial = std::shared_ptr<PBRMaterial>;
    using WeakPBRMaterial = std::weak_ptr<PBRMaterial>;
}

#endif //LAVAVK_PBRMATERIAL_H
