//
// Created by dorian on 07. 01. 2020..
//

#ifndef LAVAVK_SPECULARLIGHT_H
#define LAVAVK_SPECULARLIGHT_H

#include "light.h"

namespace LavaVk
{
    class SpecularLight : public Light
    {
    public:
        SpecularLight();
        explicit SpecularLight(const std::string_view &name);

        glm::vec3 ambient{0.2f, 0.2f, 0.2f};
        glm::vec3 specular{1.0f, 1.0f, 1.0f};
    };

    using UniqueSpecularLight = std::unique_ptr<SpecularLight>;
    using SharedSpecularLight = std::shared_ptr<SpecularLight>;
    using WeakSpecularLight = std::weak_ptr<SpecularLight>;
}


#endif //LAVAVK_SPECULARLIGHT_H
