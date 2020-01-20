//
// Created by dorian on 02. 01. 2020..
//

#include "defaultimagesource.h"

struct RgbColor
{
    char r;
    char g;
    char b;
    char a;
};

LavaVk::DefaultImageSource::DefaultImageSource(glm::vec3 color) : color(color)
{
}

gli::texture LavaVk::DefaultImageSource::load()
{
    auto tex = gli::texture2d(gli::format::FORMAT_RGBA8_UNORM_PACK8, {1, 1}, 1);
    RgbColor c
            {
                    static_cast<char>(color.r * 255),
                    static_cast<char>(color.g * 255),
                    static_cast<char>(color.b * 255),
                    static_cast<char>(255)
            };
    auto begin = reinterpret_cast<decltype(c) *>(tex.data());
    auto end = reinterpret_cast<decltype(c) *>(reinterpret_cast<char *>(tex.data()) + tex.size());
    std::fill(begin, end, c);
    return tex;
}

std::type_index LavaVk::DefaultImageSource::getType() const
{
    return typeid(DefaultImageSource);
}
