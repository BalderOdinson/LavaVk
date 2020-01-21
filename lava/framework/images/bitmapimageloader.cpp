//
// Created by oshikuru on 28. 12. 2019..
//

#include <iostream>
#include "bitmapimageloader.h"
#include <stb_image.h>

gli::texture LavaVk::BitmapImageLoader::load(std::string_view filename)
{
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(filename.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if(!pixels)
        throw std::runtime_error("Invalid image file path!");
    size_t imageSize = texWidth * texHeight * 4;
    gli::texture tex = gli::texture2d(gli::format::FORMAT_RGBA8_UNORM_PACK8, {texWidth, texHeight}, 1);
    memcpy(tex.data(), pixels, imageSize);
    stbi_image_free(pixels);
    return tex;
}

std::type_index LavaVk::BitmapImageLoader::getType() const
{
    return typeid(BitmapImageLoader);
}
