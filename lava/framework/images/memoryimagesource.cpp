//
// Created by oshikuru on 28. 12. 2019..
//

#include "memoryimagesource.h"

LavaVk::MemoryImageSource::MemoryImageSource(std::vector<uint8_t> &&data, gli::format format, gli::extent2d extent,
                                             size_t layers, size_t levels) : format(format), extent(extent),
                                                                             layers(layers), levels(levels),
                                                                             data(std::move(data))
{
}

gli::texture LavaVk::MemoryImageSource::load()
{
    gli::texture tex;
    if (layers == 1)
        tex = gli::texture2d(format, extent, levels);
    else if (layers == 6)
        tex = gli::texture_cube(format, extent, levels);
    else
        tex = gli::texture2d_array(format, extent, layers, levels);
    memcpy(tex.data(), data.data(), data.size());
    return tex;
}

std::type_index LavaVk::MemoryImageSource::getType() const
{
    return typeid(MemoryImageSource);
}

