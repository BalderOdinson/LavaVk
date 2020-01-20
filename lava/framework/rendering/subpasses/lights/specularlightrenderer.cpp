//
// Created by oshikuru on 20. 01. 2020..
//

#include "specularlightrenderer.h"
#include "lava/framework/platform/application.h"

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
            Core::BufferUniformShaderResource("LightUniform", sizeof(SpecularLightRenderer::LightUniform), 1, 0, 3,
                                              true));
    resources.push_back(
            Core::BufferUniformShaderResource("LightsInfo", sizeof(SpecularLightRenderer::DeferredLights), 1, 0, 4,
                                              true));
}
