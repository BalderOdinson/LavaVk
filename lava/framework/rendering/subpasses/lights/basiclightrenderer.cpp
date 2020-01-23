//
// Created by oshikuru on 20. 01. 2020..
//

#include "basiclightrenderer.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"
#include "lava/framework/rendering/subpasses/renderers/meshrenderer.h"


LavaVk::BasicLightRenderer::BasicLightRenderer(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    shaders[vk::ShaderStageFlagBits::eVertex] = make<DeferredVertexShader>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<DeferredFragmentShader>();
}

std::vector<LavaVk::Core::SharedShaderModule> LavaVk::BasicLightRenderer::getShaderModules()
{
    std::vector<Core::SharedShaderModule> modules;
    for (auto &shader : shaders)
        modules.emplace_back(resourceCache->requestShaderModule(shader.second));
    return modules;
}

void LavaVk::BasicLightRenderer::prepare(const LavaVk::SharedScene &s, const LavaVk::SharedCamera &c,
                                         const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    scene = s;
    camera = c;
    if (scene->hasComponent<LavaVk::Light>())
        sceneLights = scene->getComponents<LavaVk::Light>();
}

void LavaVk::BasicLightRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                      const LavaVk::DepthStencilState &depthStencilState,
                                      const std::vector<uint32_t> &inputAttachments,
                                      const std::vector<uint32_t> &outputAttachments,
                                      const std::vector<uint32_t> &resolveAttachments,
                                      const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto lightBuffer = allocateLights(sceneLights, Constants::Light::MaxDeferredLightsCount, currentFrame, threadIndex);
    commandBuffer->bindBuffer(lightBuffer.getBuffer(), lightBuffer.getOffset(), lightBuffer.getSize(), 0, 4, 0);

    // Create pipeline layout and bind it
    auto pipelineLayout = resourceCache->requestPipelineLayout(make<Core::PipelineLayoutOptions>(getShaderModules()));
    commandBuffer->bindPipelineLayout(pipelineLayout);

    // Get image views of the attachments
    auto renderTarget = currentFrame->getRenderTarget();
    auto targetViews = renderTarget->getViews();

    // Bind depth, albedo, and normal as input attachments
    auto depthView = targetViews.at(1);
    commandBuffer->bindInput(depthView, 0, 0, 0);

    auto albedoView = targetViews.at(2);
    commandBuffer->bindInput(albedoView, 0, 1, 0);

    auto normalView = targetViews.at(3);
    commandBuffer->bindInput(normalView, 0, 2, 0);

    // Set cull mode to front as full screen triangle is clock-wise
    RasterizationState rasterizationState;
    rasterizationState.cullMode = vk::CullModeFlagBits::eFront;
    commandBuffer->setRasterizationState(rasterizationState);

    // Set vertex input state
    commandBuffer->setVertexInputState({});

    // Populate uniform values
    LightUniform lightUniform{};

    // Inverse resolution
    lightUniform.invResolution.x = 1.0f / renderTarget->getExtent().width;
    lightUniform.invResolution.y = 1.0f / renderTarget->getExtent().height;

    // Inverse view projection
    lightUniform.invView = glm::inverse(camera->getView());
    lightUniform.invProjection = glm::inverse(MeshRenderer::vulkanStyleProjection(camera->getProjection()));

    // Allocate a buffer using the buffer pool from the active frame to store uniform values and bind it
    auto allocation = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer, sizeof(LightUniform));
    allocation.update(lightUniform);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 3, 0);

    // Draw full screen triangle triangles
    commandBuffer->draw(3, 1, 0, 0);
}

LavaVk::BufferAllocation
LavaVk::BasicLightRenderer::allocateLights(const std::vector<SharedLight> &sceneLights, size_t maxLights,
                                           const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    if (sceneLights.size() > maxLights)
        Application::instance->container.resolve<Logger>()->warning("Exceeding max light capacity!");

    DeferredLights lightInfo{};
    lightInfo.count = static_cast<uint32_t>(std::min(sceneLights.size(), maxLights));

    std::transform(sceneLights.begin(), sceneLights.begin() + lightInfo.count, lightInfo.lights,
                   [](const SharedLight &light) -> BasicLightRenderer::Light
                   {
                       auto transform = light->getNode()->getTransform();

                       return {{transform->getTranslation(),                 static_cast<float>(light->lightType)},
                               {transform->getRotation() * light->direction, light->intensityConstant},
                               {light->intensityLinear,                      light->intensityQuadratic, light->innerConeAngle, light->outerConeAngle},
                               {light->color,                                0}};
                   });

    auto lightBuffer = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                                    sizeof(DeferredLights), threadIndex);
    lightBuffer.update(lightInfo);
    return lightBuffer;
}

std::type_index LavaVk::BasicLightRenderer::getType() const
{
    return typeid(BasicLightRenderer);
}

LavaVk::DeferredVertexShader::DeferredVertexShader()
{
    filename = "shaders/deferred.basic.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::OutputShaderResource("o_uv", 2, 1, 1, 0));
}

LavaVk::DeferredFragmentShader::DeferredFragmentShader()
{
    filename = "shaders/deferred.basic.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_uv", 2, 1, 1, 0));
    resources.push_back(Core::OutputShaderResource("o_color", 4, 1, 1, 0));
    resources.push_back(Core::InputAttachmentShaderResource("in_depth", 1, 0, 0, 0));
    resources.push_back(Core::InputAttachmentShaderResource("in_albedo", 1, 1, 0, 1));
    resources.push_back(Core::InputAttachmentShaderResource("in_normal", 1, 2, 0, 2));
    resources.push_back(
            Core::BufferUniformShaderResource("LightUniform", sizeof(BasicLightRenderer::LightUniform), 1, 0, 3, true));
    resources.push_back(
            Core::BufferUniformShaderResource("LightsInfo", sizeof(BasicLightRenderer::DeferredLights), 1, 0, 4, true));
}
