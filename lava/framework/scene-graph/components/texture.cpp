//
// Created by dorian on 14. 12. 2019..
//

#include "texture.h"
#include "lava/framework/images/defaultimagesource.h"

#include <utility>

std::type_index LavaVk::Texture::getType() const
{
    return typeid(Texture);
}

LavaVk::SharedImage LavaVk::Texture::getImage() const
{
    return image;
}

LavaVk::SharedSampler LavaVk::Texture::getSampler() const
{
    return sampler;
}

LavaVk::Texture::Texture(std::string_view name) : Component(name), image(make<Image>(make<DefaultImageSource>(),
                                                                                       make<MipmapsOptions>(0,
                                                                                                            LevelOfDetail::VeryHigh),
                                                                                       name)),
                                                    sampler(make<Sampler>(1, name))
{
}

LavaVk::Texture::Texture(LavaVk::SharedImage image, LavaVk::SharedSampler sampler, std::string_view name) : Component(
        name), image(std::move(image)), sampler(std::move(sampler))
{
}

bool LavaVk::Texture::isValid()
{
    return image && image->isValid() && sampler && sampler->isValid();
}

void LavaVk::Texture::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    if (!image->isValid())
        image->load(commandBuffer);

    if (!sampler->isValid())
    {
        sampler->setLod(image->getMipmaps().size(), image->getMipmapsOptions());
        sampler->load(commandBuffer);
    }
}
