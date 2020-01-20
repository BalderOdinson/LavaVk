//
// Created by oshikuru on 28. 12. 2019..
//

#ifndef LAVAVK_MEMORYIMAGESOURCE_H
#define LAVAVK_MEMORYIMAGESOURCE_H

#include <vector>
#include "imagesource.h"

namespace LavaVk
{
    class MemoryImageSource : public ImageSource
    {
    public:
        MemoryImageSource(std::vector<uint8_t>&& data, gli::format format, gli::extent2d extent, size_t layers = 1,
                          size_t levels = 1);

        gli::texture load() override;
        std::type_index getType() const override;

    private:
        gli::format format;
        gli::extent2d extent;
        size_t layers;
        size_t levels;
        std::vector<uint8_t> data;
    };
}


#endif //LAVAVK_MEMORYIMAGESOURCE_H
