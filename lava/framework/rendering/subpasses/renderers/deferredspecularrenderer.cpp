//
// Created by oshikuru on 20. 01. 2020..
//

#include "deferredspecularrenderer.h"
#include "options/deferredrendereroptions.h"

LavaVk::DeferredSpecularRenderer::DeferredSpecularRenderer(LavaVk::InjectionContext &context)
{
    auto options = context.container.option<DeferredRendererOptions>(context);
    baseRenderer = options->baseRenderer;
    Core::SharedShaderModuleOptions shader;
    if (baseRenderer->tryGetShaderOptions(vk::ShaderStageFlagBits::eFragment, shader))
    {
        auto originalName = shader->filename;
        shader->filename = originalName.substr(0, originalName.find('.')) + "." + options->shaderNameExtension +
                           originalName.substr(originalName.find('.'));
        shader->resources.push_back(
                Core::OutputShaderResource("o_specular", 4, 1, 1, 1));
        shader->resources.push_back(
                Core::OutputShaderResource("o_normal", 4, 1, 1, 2));
    }
}

std::type_index LavaVk::DeferredSpecularRenderer::getType() const
{
    return typeid(DeferredSpecularRenderer);
}

std::vector<std::pair<LavaVk::SharedNode, LavaVk::SharedComponent>>
LavaVk::DeferredSpecularRenderer::prepare(const LavaVk::SharedScene &scene, const LavaVk::SharedCamera &camera,
                                          const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    return baseRenderer->prepare(scene, camera, commandBuffer);
}

void LavaVk::DeferredSpecularRenderer::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                            const std::pair<SharedNode, SharedComponent> &component,
                                            const LavaVk::DepthStencilState &depthStencilState,
                                            const std::vector<uint32_t> &inputAttachments,
                                            const std::vector<uint32_t> &outputAttachments,
                                            const std::vector<uint32_t> &resolveAttachments,
                                            const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    return baseRenderer->draw(commandBuffer, component, depthStencilState, inputAttachments, outputAttachments,
                              resolveAttachments, currentFrame, threadIndex);
}

bool LavaVk::DeferredSpecularRenderer::tryGetShaderOptions(vk::ShaderStageFlagBits stage,
                                                           LavaVk::Core::SharedShaderModuleOptions &options)
{
    return baseRenderer->tryGetShaderOptions(stage, options);
}
