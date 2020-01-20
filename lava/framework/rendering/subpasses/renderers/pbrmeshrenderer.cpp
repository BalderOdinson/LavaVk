//
// Created by dorian on 06. 01. 2020..
//

#include "pbrmeshrenderer.h"
#include "lava/framework/resourcecache.h"
#include "lava/framework/scene-graph/components/pbrmaterial.h"
#include "lava/framework/constants.h"

LavaVk::PBRMeshRenderer::PBRMeshRenderer(LavaVk::InjectionContext &context) : MeshRenderer(context)
{
    shaders[vk::ShaderStageFlagBits::eVertex] = make<PBRVertexShaderOptions>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<PBRFragmentShaderOptions>();
}

std::type_index LavaVk::PBRMeshRenderer::getType() const
{
    return typeid(PBRMeshRenderer);
}

bool LavaVk::PBRMeshRenderer::shouldRenderSubmesh(const LavaVk::SharedSubMesh &submesh)
{
    return bool(std::dynamic_pointer_cast<PBRMaterial>(submesh->getMaterial()));
}

size_t LavaVk::PBRMeshRenderer::getSetCount() const
{
    return 6;
}

LavaVk::PBRVertexShaderOptions::PBRVertexShaderOptions()
{
    filename = "shaders/pbr.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::InputShaderResource("position", 3, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("texcoord", 2, 1, 1, 1));
    resources.push_back(Core::InputShaderResource("normal", 3, 1, 1, 2));
    resources.push_back(Core::InputShaderResource("tangent", 3, 1, 1, 3));
    /*resources.push_back(Core::InputShaderResource("bitangent", 3, 1, 1, 4));*/
    resources.push_back(Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(Core::OutputShaderResource("o_position", 3, 1, 1, 0));
    resources.push_back(Core::OutputShaderResource("o_uv", 2, 1, 1, 1));
    resources.push_back(Core::OutputShaderResource("o_normal", 3, 1, 1, 2));
    resources.push_back(Core::OutputShaderResource("o_tangent", 3, 1, 1, 3));
    /*resources.push_back(Core::OutputShaderResource("o_bitangent", 3, 1, 1, 4));*/
}

LavaVk::PBRFragmentShaderOptions::PBRFragmentShaderOptions()
{
    filename = "shaders/pbr.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_position", 3, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("in_uv", 2, 1, 1, 1));
    resources.push_back(Core::InputShaderResource("in_normal", 3, 1, 1, 2));
    resources.push_back(Core::InputShaderResource("in_tangent", 3, 1, 1, 3));
    /*resources.push_back(Core::InputShaderResource("in_bitangent", 3, 1, 1, 4));*/
    resources.push_back(Core::OutputShaderResource("o_color", 4, 1, 1, 0));
    resources.push_back(Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Diffuse, 1, 1, 0));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Emissive, 1, 1, 1));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Ambient, 1, 1, 2));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Specular, 1, 1, 3));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::SpecularHighlight, 1, 1, 4));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Normal, 1, 2, 0));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Bump, 1, 2, 1));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Displacement, 1, 2, 2));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Alpha, 1, 3, 0));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Reflection, 1, 3, 1));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Metallic, 1, 4, 0));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Roughness, 1, 4, 1));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Cubemap, 1, 5, 0));
    resources.push_back(Core::PushConstantShaderResource("PBRMaterialUniform", 0,
                                                         sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(float) +
                                                         sizeof(float) + sizeof(uint32_t)));
}
