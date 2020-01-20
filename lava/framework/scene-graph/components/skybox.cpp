//
// Created by oshikuru on 29. 12. 2019..
//

#include "lava/framework/platform/application.h"
#include "skybox.h"
#include "lava/framework/materials/basematerialsource.h"
#include "lava/framework/meshes/memorymeshsource.h"
#include "pbrmaterial.h"
#include "lava/framework/constants.h"

#include <utility>

LavaVk::Skybox::Skybox(SharedImageSource imageSource, SharedVertexSource vertexSource, std::string_view name)
        : Component(name)
{
    textures[Constants::Texture::Cubemap] = make<Texture>(
            make<Image>(std::move(imageSource), make<MipmapsOptions>(0, LevelOfDetail::VeryHigh)),
            make<Sampler>(vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
                          std::array<vk::SamplerAddressMode, 3>{vk::SamplerAddressMode::eClampToEdge,
                                                                vk::SamplerAddressMode::eClampToEdge,
                                                                vk::SamplerAddressMode::eClampToEdge}, 16.f, 1, name),
            name);
    geometry = make<Geometry>(std::move(vertexSource));
}

void LavaVk::Skybox::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    if (!geometry->isValid())
        geometry->load(cmd);

    for (auto &texture : textures)
        if(!texture.second->isValid())
            texture.second->load(cmd);
}

std::type_index LavaVk::Skybox::getType() const
{
    return typeid(Skybox);
}

LavaVk::SharedGeometry LavaVk::Skybox::getGeometry() const
{
    return geometry;
}

bool LavaVk::Skybox::isValid() const
{
    return geometry && geometry->isValid() && std::all_of(textures.begin(), textures.end(), [](const auto &texture)
    { return texture.second && texture.second->isValid(); });
}

void LavaVk::Skybox::onAddedToNode(const LavaVk::SharedNode &n)
{
    Component::onAddedToNode(n);
    node = n;
}

void LavaVk::Skybox::onRemovedFormNode(const LavaVk::SharedNode &n)
{
    Component::onRemovedFormNode(n);
    node = {};
}

LavaVk::SharedNode LavaVk::Skybox::getNode() const
{
    return node.expired() ? nullptr : node.lock();
}
