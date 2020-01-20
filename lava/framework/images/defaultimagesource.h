//
// Created by dorian on 02. 01. 2020..
//

#ifndef LAVAVK_DEFAULTIMAGESOURCE_H
#define LAVAVK_DEFAULTIMAGESOURCE_H

#include "imagesource.h"
#include "lava/third-party/glm.h"

namespace LavaVk
{
    class DefaultImageSource : public ImageSource
    {
    public:
        DefaultImageSource() = default;
        explicit DefaultImageSource(glm::vec3 color);

        gli::texture load() override;
        std::type_index getType() const override;

    private:
        glm::vec3 color{1.f, 1.f, 1.f};
    };
}


#endif //LAVAVK_DEFAULTIMAGESOURCE_H
