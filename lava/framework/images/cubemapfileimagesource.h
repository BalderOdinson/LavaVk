//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_CUBEMAPFILEIMAGESOURCE_H
#define LAVAVK_CUBEMAPFILEIMAGESOURCE_H

#include <array>
#include "imagesource.h"

namespace LavaVk
{
    class CubemapFileImageSource : public ImageSource
    {
    public:
        explicit CubemapFileImageSource(std::string_view right, std::string_view left, std::string_view up,
                                        std::string_view down, std::string_view backward, std::string_view forward);

        gli::texture load() override;
        std::type_index getType() const override;

    private:
        std::array<std::string, 6> filenames;
    };

    using UniqueCubemapFileImageSource = std::unique_ptr<CubemapFileImageSource>;
    using SharedCubemapFileImageSource = std::shared_ptr<CubemapFileImageSource>;
    using WeakCubemapFileImageSource = std::weak_ptr<CubemapFileImageSource>;
}

#endif //LAVAVK_CUBEMAPFILEIMAGESOURCE_H
