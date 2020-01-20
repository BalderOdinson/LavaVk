//
// Created by oshikuru on 11. 01. 2020..
//

#include "pathrenderer.h"
#include "options/geometryrenderoptions.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"
#include "meshrenderer.h"
#include <set>

LavaVk::PathRenderer::PathRenderer(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    auto options = context.container.option<GeometryRenderOptions>(context);
    nodeMode = options->nodeMode;
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::PathRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &c,
                              const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    camera = c;
    std::multimap<std::pair<size_t, float>, std::pair<SharedNode, SharedPathGeometry>, LayerComparer> nodes;

    auto cameraTransform = camera->getNode()->getTransform()->getWorldMatrix();

    if (scene->hasComponent<PathGeometry>())
    {
        auto paths = scene->getComponents<PathGeometry>();

        for (auto &path : paths)
        {
            if (!path->isValid())
                path->load(commandBuffer);

            for (auto &node : path->getNodes())
            {
                auto nodeTransform = node->getTransform()->getWorldMatrix();
                auto meshBounds = path->getBounds();
                AABB worldBounds(meshBounds->getMin(), meshBounds->getMax());
                worldBounds.transform(nodeTransform);

                float distance = glm::length(glm::vec3(cameraTransform[3]) - worldBounds.getCenter());

                if (path->alphaMode == nodeMode && shouldRenderPath(path))
                    nodes.emplace(std::make_pair(node->getLayer(), distance), std::make_pair(node, path));
            }
        }

        std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>> sortedNodes(nodes.size());

        std::transform(nodes.begin(), nodes.end(), sortedNodes.begin(),
                       [](const std::pair<std::pair<size_t, float>, std::pair<SharedNode, SharedPathGeometry >> &node)
                       {
                           return node.second;
                       });

        return sortedNodes;
    }

    return {};
}

void LavaVk::PathRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                const std::pair<SharedNode, SharedComponent> &component,
                                const LavaVk::DepthStencilState &depthStencilState,
                                const std::vector<uint32_t> &inputAttachments,
                                const std::vector<uint32_t> &outputAttachments,
                                const std::vector<uint32_t> &resolveAttachments,
                                const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto device = Application::instance->container.resolve<Core::Device>();

    auto path = std::dynamic_pointer_cast<PathGeometry>(component.second);

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

    // Invert the front face if the path was flipped
    auto scale = component.first->getTransform()->getScale();
    bool flipped = scale.x * scale.y * scale.z < 0;
    vk::FrontFace frontFace = flipped ? vk::FrontFace::eClockwise : vk::FrontFace::eCounterClockwise;
    RasterizationState rasterizationState{};
    rasterizationState.frontFace = frontFace;
    rasterizationState.depthBiasEnable = true;
    commandBuffer->setDepthBias(1.f, 0.f, -1.f);
    rasterizationState.cullMode = vk::CullModeFlagBits::eNone;
    rasterizationState.polygonMode = vk::PolygonMode::eLine;

    commandBuffer->setRasterizationState(rasterizationState);

    auto pipelineLayout = resourceCache->requestPipelineLayout(
            make<Core::PipelineLayoutOptions>(getShaderModules()));

    commandBuffer->bindPipelineLayout(pipelineLayout);
    updateUniforms(commandBuffer, {component.first, path}, currentFrame, threadIndex);

    auto vertexInputResources = pipelineLayout->getVertexInputAttributes();

    VertexInputState vertexInputState;

    std::set<uint32_t> usedBindings;

    for (auto &inputResource : vertexInputResources)
    {
        VertexAttribute attribute;

        if (!path->getVertexAttributes()->tryGetVertexAttribute(inputResource.name, attribute))
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
        vertexBinding.stride = path->getVertexAttributes()->getVertexInputs()[usedBinding].stride;

        vertexInputState.bindings.push_back(vertexBinding);
    }

    commandBuffer->setVertexInputState(vertexInputState);

    InputAssemblyState assemblyState{};
    assemblyState.topology = vk::PrimitiveTopology::eLineStripWithAdjacency;

    commandBuffer->setInputAssemblyState(assemblyState);

    commandBuffer->setLineWidth(path->lineWidth);

    // Find path vertex buffers matching the shader input attribute names
    for (auto &usedBinding : usedBindings)
    {
        auto buffer = path->getVertexBuffer(usedBinding);

        // Bind vertex buffers only for the attribute locations defined
        commandBuffer->bindVertexBuffers(usedBinding, {buffer}, {0});
    }

    drawPathCommand(commandBuffer, path);
}

bool LavaVk::PathRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                               LavaVk::Core::SharedShaderModuleOptions &options)
{
    if (shaders.find(stage) != shaders.end())
    {
        options = shaders[stage];
        return true;
    }
    return false;
}

std::vector<LavaVk::Core::SharedShaderModule> LavaVk::PathRenderer::getShaderModules()
{
    std::vector<Core::SharedShaderModule> modules;
    for (auto &shader : shaders)
        modules.emplace_back(resourceCache->requestShaderModule(shader.second));
    return modules;
}

void LavaVk::PathRenderer::updateUniforms(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                          const std::pair<SharedNode, SharedPathGeometry> &component,
                                          const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    MeshRenderer::Ubo globalUniform{};
    globalUniform.view = camera->getView();
    globalUniform.projection = MeshRenderer::vulkanStyleProjection(camera->getProjection());
    auto transform = component.first->getTransform();
    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(MeshRenderer::Ubo),
                                                   threadIndex);
    globalUniform.model = transform->getWorldMatrix();
    globalUniform.modelInverse = glm::transpose(glm::inverse(globalUniform.model));
    globalUniform.cameraPosition = glm::vec3(glm::inverse(camera->getView())[3]);
    allocation.update(globalUniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 0, 0);
}

void LavaVk::PathRenderer::drawPathCommand(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                           const LavaVk::SharedPathGeometry &path)
{
// Draw submesh indexed if indices exists
    IndexOptions indexOptions;
    if (path->getVertexAttributes()->tryGetIndexOptions(indexOptions))
    {
        // Bind index buffer of submesh
        commandBuffer->bindIndexBuffer(path->getIndexBuffer(), indexOptions.offset, indexOptions.type);

        // Draw submesh using indexed data
        commandBuffer->drawIndexed(path->getIndexCount(), 1, 0, 0, 0);
    }
    else
        // Draw submesh using vertices only
        commandBuffer->draw(path->getVertexCount(), 1, 0, 0);
}
