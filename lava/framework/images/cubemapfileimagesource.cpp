//
// Created by oshikuru on 28. 12. 2019..
//

#include "cubemapfileimagesource.h"
#include <stb_image.h>

LavaVk::CubemapFileImageSource::CubemapFileImageSource(std::string_view right, std::string_view left,
                                                       std::string_view up, std::string_view down,
                                                       std::string_view backward, std::string_view forward) : filenames{
        std::string(right), std::string(left), std::string(up), std::string(down), std::string(backward),
        std::string(forward)}
{
}

gli::texture LavaVk::CubemapFileImageSource::load()
{
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(filenames[0].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    size_t imageSize = texWidth * texHeight * 4;
    gli::texture tex = gli::texture_cube(gli::format::FORMAT_RGBA8_UNORM_PACK8, {texWidth, texHeight}, 1);
    memcpy(tex.data(0, 0, 0), pixels, imageSize);
    stbi_image_free(pixels);

    for (size_t i = 1; i < 6; ++i)
    {
        pixels = stbi_load(filenames[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        imageSize = texWidth * texHeight * 4;
        memcpy(tex.data(0, i, 0), pixels, imageSize);
        stbi_image_free(pixels);
    }
    return tex;
}

std::type_index LavaVk::CubemapFileImageSource::getType() const
{
    return typeid(CubemapFileImageSource);
}
