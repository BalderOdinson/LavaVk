//
// Created by dorian on 03. 01. 2020..
//

#include "objectfilematerialsource.h"

#include <utility>
#include "lava/framework/platform/application.h"
#include "lava/framework/scene-graph/components/pbrmaterial.h"
#include "lava/framework/scene-graph/components/specularmaterial.h"
#include "lava/framework/images/fileimagesource.h"
#include "lava/framework/constants.h"

bool checkIfPBR(tinyobj::material_t &material)
{
    return material.metallic != 0.f || material.roughness != 0.f || !material.metallic_texname.empty() ||
           !material.roughness_texname.empty();
}

LavaVk::ObjectFileMaterialSource::ObjectFileMaterialSource(tinyobj::material_t material, std::string_view baseDir)
        : materialSource(std::move(material)), baseDir(baseDir)
{
}

LavaVk::SharedMaterial LavaVk::ObjectFileMaterialSource::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    SharedMaterial material;
    if (checkIfPBR(materialSource))
    {
        auto pbr = make<PBRMaterial>(materialSource.name);
        pbr->metallicFactor = materialSource.metallic;
        pbr->roughnessFactor = materialSource.roughness;
        pbr->baseColorFactor = {materialSource.diffuse[0], materialSource.diffuse[1],
                                materialSource.diffuse[2], materialSource.dissolve};
        material = pbr;
    }
    else
    {
        auto specular = make<SpecularMaterial>(materialSource.name);
        specular->ambient = {materialSource.ambient[0], materialSource.ambient[1],
                             materialSource.ambient[2], 0.f};
        specular->diffuse = {materialSource.diffuse[0], materialSource.diffuse[1],
                             materialSource.diffuse[2], materialSource.dissolve};
        specular->specular = {materialSource.specular[0], materialSource.specular[1],
                              materialSource.specular[2], materialSource.shininess};
        material = specular;
    }

    material->emissive = {materialSource.emission[0], materialSource.emission[1],
                          materialSource.emission[2], 0.f};

    if (materialSource.dissolve != 1.f)
        material->alphaMode = AlphaMode::Blend;

    auto sampler = make<Sampler>();
    if (!materialSource.alpha_texname.empty())
    {
        material->textures[Constants::Texture::Alpha] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.alpha_texname)),
                sampler);
    }
    if (!materialSource.ambient_texname.empty())
    {
        material->textures[Constants::Texture::Ambient] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.ambient_texname)),
                sampler);
    }
    if (!materialSource.bump_texname.empty())
    {
        material->textures[Constants::Texture::Bump] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.bump_texname)),
                sampler);
    }
    if (!materialSource.diffuse_texname.empty())
    {
        material->textures[Constants::Texture::Diffuse] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.diffuse_texname)),
                sampler);
    }
    if (!materialSource.displacement_texname.empty())
    {
        material->textures[Constants::Texture::Displacement] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.displacement_texname)),
                sampler);
    }
    if (!materialSource.emissive_texname.empty())
    {
        material->textures[Constants::Texture::Emissive] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.emissive_texname)),
                sampler);
    }
    if (!materialSource.metallic_texname.empty())
    {
        material->textures[Constants::Texture::Metallic] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.metallic_texname)),
                sampler);
    }
    if (!materialSource.normal_texname.empty())
    {
        material->textures[Constants::Texture::Normal] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.normal_texname)),
                sampler);
    }
    if (!materialSource.reflection_texname.empty())
    {
        material->textures[Constants::Texture::Reflection] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.reflection_texname)),
                sampler);
    }
    if (!materialSource.roughness_texname.empty())
    {
        material->textures[Constants::Texture::Roughness] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.roughness_texname)),
                sampler);
    }
    if (!materialSource.specular_highlight_texname.empty())
    {
        material->textures[Constants::Texture::SpecularHighlight] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.specular_highlight_texname)),
                sampler);
    }
    if (!materialSource.specular_texname.empty())
    {
        material->textures[Constants::Texture::Specular] = make<Texture>(
                make<Image>(make<FileImageSource>(baseDir + materialSource.specular_texname)),
                sampler);
    }

    for (auto &tex : material->textures)
        tex.second->load(cmd);

    return material;
}

std::type_index LavaVk::ObjectFileMaterialSource::getType() const
{
    return typeid(ObjectFileMaterialSource);
}
