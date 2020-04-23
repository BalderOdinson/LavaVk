//
// Created by dorian on 05. 01. 2020..
//

#include "meshrenderer.h"
#include <set>
#include <lava/framework/core/options/pipelinelayoutoptions.h>
#include "lava/framework/rendering/subpasses/renderers/options/geometryrenderoptions.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/scene-graph/components/mesh.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"

glm::mat4 LavaVk::MeshRenderer::vulkanStyleProjection(const glm::mat4 &proj)
{
    // Flip Y in clipspace. X = -1, Y = -1 is topLeft in Vulkan.
    glm::mat4 mat = proj;
    mat[1][1] *= -1;

    return mat;
}

LavaVk::MeshRenderer::MeshRenderer(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    auto options = context.container.option<GeometryRenderOptions>(context);
    nodeMode = options->nodeMode;
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::MeshRenderer::prepare(const SharedScene &scene, const SharedCamera &c,
                              const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    if (!defaultTexture)
    {
        defaultTexture = make<Texture>();
        // TODO: Assumes that given command buffer will be submitted to the queue that supports transfer.
        defaultTexture->load(commandBuffer);
    }

    camera = c;
    if (scene->hasComponent<Skybox>())
        skybox = scene->getComponents<Skybox>().front();
    std::multimap<std::pair<size_t, float>, std::pair<SharedNode, SharedSubMesh>, LayerComparer> nodes;

    auto cameraTransform = camera->getNode()->getTransform()->getWorldMatrix();

    if(scene->hasComponent<Mesh>())
    {
        auto meshes = scene->getComponents<Mesh>();

        for (auto &mesh : meshes)
        {
            if (!mesh->isValid())
                mesh->load(commandBuffer);

            for (auto &node : mesh->getNodes())
            {
                auto nodeTransform = node->getTransform()->getWorldMatrix();
                auto meshBounds = mesh->getBounds();
                AABB worldBounds(meshBounds->getMin(), meshBounds->getMax());
                worldBounds.transform(nodeTransform);

                float distance = glm::length(glm::vec3(cameraTransform[3]) - worldBounds.getCenter());

                for (auto &subMesh : mesh->getSubmeshes())
                {
                    if (subMesh->getMaterial()->alphaMode == nodeMode && shouldRenderSubmesh(subMesh))
                        nodes.emplace(std::make_pair(node->getLayer(), distance), std::make_pair(node, subMesh));
                }
            }
        }

        std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>> sortedNodes(nodes.size());

        std::transform(nodes.begin(), nodes.end(), sortedNodes.begin(),
                       [](const std::pair<std::pair<size_t, float>, std::pair<SharedNode, SharedSubMesh>> &node)
                       {
                           return node.second;
                       });

        return sortedNodes;
    }

    return {};
}

void
LavaVk::MeshRenderer::draw(const Core::SharedCommandBuffer &commandBuffer,
                           const std::pair<SharedNode, SharedComponent> &component,
                           const DepthStencilState &depthStencilState, const std::vector<uint32_t> &inputAttachments,
                           const std::vector<uint32_t> &outputAttachments,
                           const std::vector<uint32_t> &resolveAttachments,
                           const SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto device = Application::instance->container.resolve<Core::Device>();

    auto submesh = std::dynamic_pointer_cast<SubMesh>(component.second);

    if (nodeMode == AlphaMode::Blend)
    {
        // Enable alpha blending
        ColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;

        ColorBlendState colorBlendState{};
        colorBlendState.attachments.resize(outputAttachments.size());
        colorBlendState.attachments[0] = colorBlendAttachment;
        commandBuffer->setColorBlendState(colorBlendState);

        commandBuffer->setDepthStencilState(depthStencilState);
    }

    // Invert the front face if the mesh was flipped
    auto scale = component.first->getTransform()->getScale();
    bool flipped = scale.x * scale.y * scale.z < 0;
    vk::FrontFace frontFace = flipped ? vk::FrontFace::eClockwise : vk::FrontFace::eCounterClockwise;
    RasterizationState rasterizationState{};
    rasterizationState.frontFace = frontFace;
    rasterizationState.depthBiasEnable = true;
    commandBuffer->setDepthBias(1.f, 0.f, -1.f);

    if (submesh->getMaterial()->doubleSided)
        rasterizationState.cullMode = vk::CullModeFlagBits::eNone;

    commandBuffer->setRasterizationState(rasterizationState);

    InputAssemblyState assemblyState{};
    commandBuffer->setInputAssemblyState(assemblyState);

    commandBuffer->setLineWidth(1.f);

    auto pipelineLayout = resourceCache->requestPipelineLayout(
            make<Core::PipelineLayoutOptions>(getShaderModules()));

    commandBuffer->bindPipelineLayout(pipelineLayout);

    std::unordered_set<std::string> defaultTextures;
    updateUniforms(commandBuffer, {component.first, submesh}, currentFrame, threadIndex);

    for (size_t set = 0; set < getSetCount(); ++set)
    {
        auto descriptorSetLayout = pipelineLayout->getSetLayout(set);

        for (auto &defaultTextureName : defaultTextures)
        {
            vk::DescriptorSetLayoutBinding layoutBinding;

            if (descriptorSetLayout->tryGetLayoutBinding(defaultTextureName, layoutBinding))
            {
                commandBuffer->bindImage(defaultTexture->getImage()->getVkImageView(),
                                         defaultTexture->getSampler()->getVkSampler(),
                                         set, layoutBinding.binding, 0);
            }
        }

        for (auto &texture : submesh->getMaterial()->textures)
        {
            vk::DescriptorSetLayoutBinding layoutBinding;

            if (descriptorSetLayout->tryGetLayoutBinding(texture.first, layoutBinding))
            {
                commandBuffer->bindImage(texture.second->getImage()->getVkImageView(),
                                         texture.second->getSampler()->getVkSampler(),
                                         set, layoutBinding.binding, 0);
            }
        }
    }

    auto vertexInputResources = pipelineLayout->getVertexInputAttributes();

    VertexInputState vertexInputState;

    std::set<uint32_t> usedBindings;

    for (auto &inputResource : vertexInputResources)
    {
        VertexAttribute attribute;

        if (!submesh->getGeometry()->getVertexAttributes()->tryGetVertexAttribute(inputResource.name, attribute))
            continue;

        vk::VertexInputAttributeDescription vertexAttribute{};
        vertexAttribute.binding = attribute.binding;
        vertexAttribute.format = attribute.format;
        vertexAttribute.location = inputResource.location;
        vertexAttribute.offset = attribute.offset;

        vertexInputState.attributes.push_back(vertexAttribute);

        usedBindings.insert(attribute.binding);
    }

    for (auto &usedBinding : usedBindings)
    {
        vk::VertexInputBindingDescription vertexBinding{};
        vertexBinding.binding = usedBinding;
        vertexBinding.stride = submesh->getGeometry()->getVertexAttributes()->getVertexInputs()[usedBinding].stride;

        vertexInputState.bindings.push_back(vertexBinding);
    }

    commandBuffer->setVertexInputState(vertexInputState);

    // Find submesh vertex buffers matching the shader input attribute names
    for (auto &usedBinding : usedBindings)
    {
        auto buffer = submesh->getGeometry()->getVertexBuffer(usedBinding);

        // Bind vertex buffers only for the attribute locations defined
        commandBuffer->bindVertexBuffers(usedBinding, {buffer}, {0});
    }

    drawSubmeshCommand(commandBuffer, submesh);
}

void LavaVk::MeshRenderer::updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                                          const std::pair<SharedNode, SharedSubMesh> &component,
                                          const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    Ubo globalUniform{};
    globalUniform.view = camera->getView();
    globalUniform.projection = vulkanStyleProjection(camera->getProjection());
    auto transform = component.first->getTransform();
    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(Ubo),
                                                   threadIndex);
    globalUniform.model = transform->getWorldMatrix();
    globalUniform.modelInverse = glm::transpose(glm::inverse(globalUniform.model));
    globalUniform.cameraPosition = glm::vec3(glm::inverse(camera->getView())[3]);
    allocation.update(globalUniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 0, 0);

    MaterialFlags flags{};
    auto material = component.second->getMaterial();
    defaultTextures.clear();

    if (material->textures.find(Constants::Texture::Alpha) != material->textures.end())
        flags = flags | MaterialFlags::HasAlpha;

    if (material->textures.find(Constants::Texture::Ambient) != material->textures.end())
        flags = flags | MaterialFlags::HasAmbient;

    if (material->textures.find(Constants::Texture::Bump) != material->textures.end())
        flags = flags | MaterialFlags::HasBump;

    if (material->textures.find(Constants::Texture::Diffuse) != material->textures.end())
        flags = flags | MaterialFlags::HasDiffuse;

    if (material->textures.find(Constants::Texture::Displacement) != material->textures.end())
        flags = flags | MaterialFlags::HasDisplacement;

    if (material->textures.find(Constants::Texture::Emissive) != material->textures.end())
        flags = flags | MaterialFlags::HasEmissive;

    if (material->textures.find(Constants::Texture::Metallic) != material->textures.end())
        flags = flags | MaterialFlags::HasMetallic;

    if (material->textures.find(Constants::Texture::Normal) != material->textures.end())
        flags = flags | MaterialFlags::HasNormal;

    if (material->textures.find(Constants::Texture::Reflection) != material->textures.end())
        flags = flags | MaterialFlags::HasReflection;

    if (material->textures.find(Constants::Texture::Roughness) != material->textures.end())
        flags = flags | MaterialFlags::HasRoughness;

    if (material->textures.find(Constants::Texture::SpecularHighlight) != material->textures.end())
        flags = flags | MaterialFlags::HasSpecularHighlight;

    if (material->textures.find(Constants::Texture::Specular) != material->textures.end())
        flags = flags | MaterialFlags::HasSpecular;

    if (material->textures.find(Constants::Texture::Cubemap) != material->textures.end())
        flags = flags | MaterialFlags::HasCubemap;
    else if (skybox)
    {
        flags = flags | MaterialFlags::HasCubemap;
        material->textures[Constants::Texture::Cubemap] = skybox->textures[Constants::Texture::Cubemap];
    }

    std::vector<uint8_t> flagsData(sizeof(uint32_t));
    *reinterpret_cast<uint32_t *>(flagsData.data()) = static_cast<uint32_t>(flags);
    auto data = material->data();
    data.insert(data.end(), flagsData.begin(), flagsData.end());
    commandBuffer->pushConstants(0, data);
}

void LavaVk::MeshRenderer::drawSubmeshCommand(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                              const LavaVk::SharedSubMesh &subMesh)
{
    // Draw submesh indexed if indices exists
    IndexOptions indexOptions;
    if (subMesh->getGeometry()->getVertexAttributes()->tryGetIndexOptions(indexOptions))
    {
        // Bind index buffer of submesh
        commandBuffer->bindIndexBuffer(subMesh->getGeometry()->getIndexBuffer(), indexOptions.offset, indexOptions.type);

        // Draw submesh using indexed data
        commandBuffer->drawIndexed(subMesh->getGeometry()->getIndexCount(), 1, 0, 0, 0);
    }
    else
        // Draw submesh using vertices only
        commandBuffer->draw(subMesh->getGeometry()->getVertexCount(), 1, 0, 0);
}

std::vector<LavaVk::Core::SharedShaderModule> LavaVk::MeshRenderer::getShaderModules()
{
    std::vector<Core::SharedShaderModule> modules;
    for (auto &shader : shaders)
        modules.emplace_back(resourceCache->requestShaderModule(shader.second));
    return modules;
}

bool LavaVk::MeshRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                               LavaVk::Core::SharedShaderModuleOptions &options)
{
    if (shaders.find(stage) != shaders.end())
    {
        options = shaders[stage];
        return true;
    }
    return false;
}
