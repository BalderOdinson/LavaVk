//
// Created by dorian on 08. 01. 2020..
//

#include "skyboxrenderer.h"
#include "lava/framework/scene-graph/scene.h"
#include "meshrenderer.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"
#include "lava/framework/constants.h"
#include <set>

LavaVk::SkyboxRenderer::SkyboxRenderer(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    shaders[vk::ShaderStageFlagBits::eVertex] = make<SkyboxVertexShaderOptions>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<SkyboxFragmentShaderOptions>();
}

std::type_index LavaVk::SkyboxRenderer::getType() const
{
    return typeid(SkyboxRenderer);
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::SkyboxRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &c,
                                const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    camera = c;

    if(!scene->hasComponent<Skybox>())
        throw std::runtime_error("Skybox renderer provided but no valid skybox available!");

    auto s = scene->getComponents<Skybox>();
    if (!s.empty())
    {
        skybox = s.front();
        if (!skybox->isValid())
            skybox->load(commandBuffer);
        return {{skybox->getNode(), skybox}};
    }
    return {};
}

void LavaVk::SkyboxRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                  const std::pair<SharedNode, SharedComponent> &component,
                                  const LavaVk::DepthStencilState &depthStencilState,
                                  const std::vector<uint32_t> &inputAttachments,
                                  const std::vector<uint32_t> &outputAttachments,
                                  const std::vector<uint32_t> &resolveAttachments,
                                  const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto device = Application::instance->container.resolve<Core::Device>();

    vk::FrontFace frontFace = vk::FrontFace::eCounterClockwise;
    RasterizationState rasterizationState{};
    rasterizationState.frontFace = frontFace;

    commandBuffer->setDepthBias(0.f, 0.f, 0.f);

    commandBuffer->setRasterizationState(rasterizationState);

    InputAssemblyState assemblyState{};
    commandBuffer->setInputAssemblyState(assemblyState);

    commandBuffer->setLineWidth(1.f);

    auto pipelineLayout = resourceCache->requestPipelineLayout(
            make<Core::PipelineLayoutOptions>(getShaderModules()));

    commandBuffer->bindPipelineLayout(pipelineLayout);
    updateUniforms(commandBuffer, currentFrame, threadIndex);

    for (size_t set = 0; set < getSetCount(); ++set)
    {
        auto descriptorSetLayout = pipelineLayout->getSetLayout(set);

        for (auto &texture : skybox->textures)
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

        if (!skybox->getGeometry()->getVertexAttributes()->tryGetVertexAttribute(inputResource.name, attribute))
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
        vertexBinding.stride = skybox->getGeometry()->getVertexAttributes()->getVertexInputs()[usedBinding].stride;

        vertexInputState.bindings.push_back(vertexBinding);
    }

    commandBuffer->setVertexInputState(vertexInputState);

    // Find geometry vertex buffers matching the shader input attribute names
    for (auto &usedBinding : usedBindings)
    {
        auto buffer = skybox->getGeometry()->getVertexBuffer(usedBinding);

        // Bind vertex buffers only for the attribute locations defined
        commandBuffer->bindVertexBuffers(usedBinding, {buffer}, {0});
    }

    drawGeometryCommand(commandBuffer, skybox->getGeometry());
}

bool LavaVk::SkyboxRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                                 LavaVk::Core::SharedShaderModuleOptions &options)
{
    if (shaders.find(stage) != shaders.end())
    {
        options = shaders[stage];
        return true;
    }
    return false;
}

void LavaVk::SkyboxRenderer::updateUniforms(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                            const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    Ubo globalUniform{};
    globalUniform.view = glm::mat4(glm::mat3(camera->getView()));
    globalUniform.projection = MeshRenderer::vulkanStyleProjection(camera->getProjection());
    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(Ubo),
                                                   threadIndex);
    allocation.update(globalUniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 0, 0);
}

size_t LavaVk::SkyboxRenderer::getSetCount() const
{
    return 1;
}

std::vector<LavaVk::Core::SharedShaderModule> LavaVk::SkyboxRenderer::getShaderModules()
{
    std::vector<Core::SharedShaderModule> modules;
    for (auto &shader : shaders)
        modules.emplace_back(resourceCache->requestShaderModule(shader.second));
    return modules;
}

void LavaVk::SkyboxRenderer::drawGeometryCommand(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                 const LavaVk::SharedGeometry &geometry)
{
    // Draw geometry indexed if indices exists
    IndexOptions indexOptions;
    if (geometry->getVertexAttributes()->tryGetIndexOptions(indexOptions))
    {
        // Bind index buffer of geometry
        commandBuffer->bindIndexBuffer(geometry->getIndexBuffer(), indexOptions.offset, indexOptions.type);

        // Draw geometry using indexed data
        commandBuffer->drawIndexed(geometry->getIndexCount(), 1, 0, 0, 0);
    }
    else
        // Draw geometry using vertices only
        commandBuffer->draw(geometry->getVertexCount(), 1, 0, 0);
}

LavaVk::SkyboxVertexShaderOptions::SkyboxVertexShaderOptions()
{
    filename = "shaders/skybox.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::InputShaderResource("position", 3, 1, 1, 0));
    resources.push_back(
            Core::BufferUniformShaderResource("UniformBufferObject", sizeof(SkyboxRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(Core::OutputShaderResource("o_texcoord", 3, 1, 1, 0));
}

LavaVk::SkyboxFragmentShaderOptions::SkyboxFragmentShaderOptions()
{
    filename = "shaders/skybox.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_texcoord", 3, 1, 1, 0));
    resources.push_back(Core::ImageSamplerShaderResource(Constants::Texture::Cubemap, 1, 0, 1));
    resources.push_back(Core::OutputShaderResource("frag_color", 4, 1, 1, 0));
}
