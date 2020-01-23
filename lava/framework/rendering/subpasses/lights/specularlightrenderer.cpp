//
// Created by oshikuru on 20. 01. 2020..
//

#include "specularlightrenderer.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"
#include "lava/framework/rendering/subpasses/renderers/meshrenderer.h"

LavaVk::SpecularLightRenderer::SpecularLightRenderer(LavaVk::InjectionContext &context) : BasicLightRenderer(context)
{
    shaders[vk::ShaderStageFlagBits::eVertex] = make<DeferredSpecularVertexShader>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<DeferredSpecularFragmentShader>();
}

LavaVk::BufferAllocation
LavaVk::SpecularLightRenderer::allocateLights(const std::vector<SharedLight> &sceneLights, size_t maxLights,
                                              const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    if (sceneLights.size() > maxLights)
        Application::instance->container.resolve<Logger>()->warning("Exceeding max light capacity!");

    SpecularLightRenderer::DeferredLights lightInfo{};
    lightInfo.count = static_cast<uint32_t>(std::min(sceneLights.size(), maxLights));

    std::transform(sceneLights.begin(), sceneLights.begin() + lightInfo.count, lightInfo.lights,
                   [](const SharedLight &l) -> SpecularLightRenderer::Light
                   {
                       auto light = std::dynamic_pointer_cast<SpecularLight>(l);
                       auto transform = light->getNode()->getTransform();

                       return {{transform->getTranslation(),                 static_cast<float>(light->lightType)},
                               {transform->getRotation() * light->direction, light->intensityConstant},
                               {light->intensityLinear,                      light->intensityQuadratic, light->innerConeAngle, light->outerConeAngle},
                               {light->color,                                0},
                               {light->ambient,                              0},
                               {light->specular,                             0}};
                   });

    auto lightBuffer = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                                    sizeof(SpecularLightRenderer::DeferredLights), threadIndex);
    lightBuffer.update(lightInfo);
    return lightBuffer;
}

void LavaVk::SpecularLightRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &camera,
                                            const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    BasicLightRenderer::prepare(scene, camera, commandBuffer);

    std::vector<SharedLight> newLights;

    for(auto &light : sceneLights)
    {
        auto specularLight = std::dynamic_pointer_cast<SpecularLight>(light);
        if(specularLight)
            newLights.push_back(specularLight);
    }

    sceneLights = newLights;
}

std::type_index LavaVk::SpecularLightRenderer::getType() const
{
    return typeid(SpecularLightRenderer);
}

void LavaVk::SpecularLightRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                         const LavaVk::DepthStencilState &depthStencilState,
                                         const std::vector<uint32_t> &inputAttachments,
                                         const std::vector<uint32_t> &outputAttachments,
                                         const std::vector<uint32_t> &resolveAttachments,
                                         const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto lightBuffer = allocateLights(sceneLights, Constants::Light::MaxDeferredLightsCount, currentFrame, threadIndex);
    commandBuffer->bindBuffer(lightBuffer.getBuffer(), lightBuffer.getOffset(), lightBuffer.getSize(), 0, 5, 0);

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

    auto specularView = targetViews.at(3);
    commandBuffer->bindInput(specularView, 0, 2, 0);

    auto normalView = targetViews.at(4);
    commandBuffer->bindInput(normalView, 0, 3, 0);

    // Set cull mode to front as full screen triangle is clock-wise
    RasterizationState rasterizationState;
    rasterizationState.cullMode = vk::CullModeFlagBits::eFront;
    commandBuffer->setRasterizationState(rasterizationState);

    // Set vertex input state
    commandBuffer->setVertexInputState({});

    commandBuffer->setLineWidth(1.f);

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
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), 0, 4, 0);

    // Draw full screen triangle triangles
    commandBuffer->draw(3, 1, 0, 0);
}

LavaVk::DeferredSpecularVertexShader::DeferredSpecularVertexShader()
{
    filename = "shaders/deferred.specular.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::OutputShaderResource("o_uv", 2, 1, 1, 0));
}

LavaVk::DeferredSpecularFragmentShader::DeferredSpecularFragmentShader()
{
    filename = "shaders/deferred.specular.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_uv", 2, 1, 1, 0));
    resources.push_back(Core::OutputShaderResource("o_color", 4, 1, 1, 0));
    resources.push_back(Core::InputAttachmentShaderResource("in_depth", 1, 0, 0, 0));
    resources.push_back(Core::InputAttachmentShaderResource("in_albedo", 1, 1, 0, 1));
    resources.push_back(Core::InputAttachmentShaderResource("in_specular", 1, 2, 0, 2));
    resources.push_back(Core::InputAttachmentShaderResource("in_normal", 1, 3, 0, 3));
    resources.push_back(
            Core::BufferUniformShaderResource("LightUniform", sizeof(SpecularLightRenderer::LightUniform), 1, 0, 4,
                                              false));
    resources.push_back(
            Core::BufferUniformShaderResource("LightsInfo", sizeof(SpecularLightRenderer::DeferredLights), 1, 0, 5,
                                              false));
}
