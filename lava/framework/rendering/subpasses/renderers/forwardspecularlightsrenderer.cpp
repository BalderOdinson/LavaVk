//
// Created by dorian on 07. 01. 2020..
//

#include "forwardspecularlightsrenderer.h"
#include "options/forwardlightsrendereroptions.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/platform/application.h"

LavaVk::ForwardSpecularLightsRenderer::ForwardSpecularLightsRenderer(LavaVk::InjectionContext &context)
{
    auto options = context.container.option<ForwardLightsRendererOptions>(context);
    baseRenderer = options->baseRenderer;
    set = options->set;
    binding = options->binding;
    Core::SharedShaderModuleOptions shader;
    if (options->stages & vk::ShaderStageFlagBits::eVertex)
    {
        if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eVertex, shader))
        {
            auto originalName = shader->filename;
            shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                               originalName.substr(originalName.find('.'));
            shader->resources.push_back(
                    Core::BufferUniformShaderResource("LightsInfo", sizeof(ForwardLights), 1, set, binding, true));
        }
    }
    if (options->stages & vk::ShaderStageFlagBits::eTessellationControl)
    {
        if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eTessellationControl, shader))
        {
            auto originalName = shader->filename;
            shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                               originalName.substr(originalName.find('.'));
            shader->resources.push_back(
                    Core::BufferUniformShaderResource("LightsInfo", sizeof(ForwardLights), 1, set, binding, true));
        }
    }
    if (options->stages & vk::ShaderStageFlagBits::eTessellationEvaluation)
    {
        if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eTessellationEvaluation, shader))
        {
            auto originalName = shader->filename;
            shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                               originalName.substr(originalName.find('.'));
            shader->resources.push_back(
                    Core::BufferUniformShaderResource("LightsInfo", sizeof(ForwardLights), 1, set, binding, true));
        }
    }
    if (options->stages & vk::ShaderStageFlagBits::eGeometry)
    {
        if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eGeometry, shader))
        {
            auto originalName = shader->filename;
            shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                               originalName.substr(originalName.find('.'));
            shader->resources.push_back(
                    Core::BufferUniformShaderResource("LightsInfo", sizeof(ForwardLights), 1, set, binding, true));
        }
    }
    if (options->stages & vk::ShaderStageFlagBits::eFragment)
    {
        if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eFragment, shader))
        {
            auto originalName = shader->filename;
            shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                               originalName.substr(originalName.find('.'));
            shader->resources.push_back(
                    Core::BufferUniformShaderResource("LightsInfo", sizeof(ForwardLights), 1, set, binding, true));
        }
    }
}

std::type_index LavaVk::ForwardSpecularLightsRenderer::getType() const
{
    return typeid(ForwardSpecularLightsRenderer);
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::ForwardSpecularLightsRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &camera,
                                               const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    lights.clear();
    if (scene->hasComponent<Light>())
    {
        for (auto &light : scene->getComponents<Light>())
        {
            auto specularLight = std::dynamic_pointer_cast<SpecularLight>(light);
            if (specularLight)
                lights.emplace_back(specularLight);
        }
    }
    return baseRenderer->prepare(scene, camera, commandBuffer);
}

void LavaVk::ForwardSpecularLightsRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                 const std::pair<SharedNode, SharedComponent> &component,
                                                 const LavaVk::DepthStencilState &depthStencilState,
                                                 const std::vector<uint32_t> &inputAttachments,
                                                 const std::vector<uint32_t> &outputAttachments,
                                                 const std::vector<uint32_t> &resolveAttachments,
                                                 const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    auto allocation = allocateLights(lights, Constants::Light::MaxForwardLightsCount, currentFrame,
                                     threadIndex);
    commandBuffer->bindBuffer(allocation.getBuffer(), allocation.getOffset(), allocation.getSize(), set, binding, 0);
    baseRenderer->draw(commandBuffer, component, depthStencilState, inputAttachments, outputAttachments,
                       resolveAttachments, currentFrame, threadIndex);
}

bool LavaVk::ForwardSpecularLightsRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                                                LavaVk::Core::SharedShaderModuleOptions &options)
{
    return baseRenderer->tryGetShaderOptions(stage, options);
}

LavaVk::BufferAllocation
LavaVk::ForwardSpecularLightsRenderer::allocateLights(const std::vector<SharedSpecularLight> &sceneLights,
                                                      size_t maxLights,
                                                      const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    if(sceneLights.size() > maxLights)
        Application::instance->container.resolve<Logger>()->warning("Exceeding max light capacity!");

    ForwardLights lightInfo{};
    lightInfo.count = static_cast<uint32_t>(std::min(sceneLights.size() , maxLights));

    std::transform(sceneLights.begin(), sceneLights.begin() + lightInfo.count, lightInfo.lights,
                   [](const SharedSpecularLight &light) -> LightUniform
                   {
                       auto transform = light->getNode()->getTransform();

                       return {{transform->getTranslation(), static_cast<float>(light->lightType)},
                               {transform->getRotation() * light->direction, light->intensityConstant},
                               {light->intensityLinear, light->intensityQuadratic, light->innerConeAngle, light->outerConeAngle},
                               {light->color, 0},
                               {light->ambient, 0},
                               {light->specular, 0}};
                   });

    auto lightBuffer = currentFrame->allocateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                                    sizeof(ForwardLights), threadIndex);
    lightBuffer.update(lightInfo);
    return lightBuffer;
}
