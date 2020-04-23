//
// Created by oshikuru on 07. 02. 2020..
//

#include "dynamicstateconfigurator.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/resourcecache.h"

void LavaVk::DynamicStateConfigurator::setDynamicStates(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    setViewportState(cmd);
    setVertexInputState(cmd);
    setInputAssemblyState(cmd);
    setRasterizationState(cmd);
    setMultisampleState(cmd);
    setDepthStencilState(cmd);
    setColorBlendState(cmd);
    setViewport(cmd);
    setScissor(cmd);
    setLineWidth(cmd);
    setDepthBias(cmd);
    setBlendConstants(cmd);
    setDepthBounds(cmd);
}

void LavaVk::DynamicStateConfigurator::setViewportState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setViewportState({});
}

void LavaVk::DynamicStateConfigurator::setVertexInputState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setVertexInputState({});
}

void LavaVk::DynamicStateConfigurator::setInputAssemblyState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setInputAssemblyState({});
}

void LavaVk::DynamicStateConfigurator::setRasterizationState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setRasterizationState({});
}

void LavaVk::DynamicStateConfigurator::setMultisampleState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto multisamplingOptions = Application::instance->container.option<MultisamplingOptions>();
    MultisampleState multisampleState{};
    multisampleState.rasterizationSamples = multisamplingOptions->samples;
    cmd->setMultisampleState(multisampleState);
}

void LavaVk::DynamicStateConfigurator::setDepthStencilState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setDepthStencilState({});
}

void LavaVk::DynamicStateConfigurator::setColorBlendState(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    ColorBlendState state{};
    state.attachments.resize(
            std::max(Application::instance->container.resolve<RenderPipeline>()->getActiveSubpass()->getOutputAttachments().size() - 1,
                     static_cast<size_t>(1)));
    cmd->setColorBlendState(state);
}

void LavaVk::DynamicStateConfigurator::setViewport(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto extent = Application::instance->container.resolve<RenderContext>()->getActiveFrame()->getRenderTarget()->getExtent();

    vk::Viewport viewport{};
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    cmd->setViewport(0, {viewport});
}

void LavaVk::DynamicStateConfigurator::setScissor(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto extent = Application::instance->container.resolve<RenderContext>()->getActiveFrame()->getRenderTarget()->getExtent();

    vk::Rect2D scissor{};
    scissor.extent = extent;
    cmd->setScissor(0, {scissor});
}

void LavaVk::DynamicStateConfigurator::setLineWidth(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setLineWidth(1.f);
}

void LavaVk::DynamicStateConfigurator::setDepthBias(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setDepthBias(0.f, 0.f, 0.f);
}

void LavaVk::DynamicStateConfigurator::setBlendConstants(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setBlendConstants({});
}

void LavaVk::DynamicStateConfigurator::setDepthBounds(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    cmd->setDepthBounds(0.f, 1.f);
}

std::type_index LavaVk::DynamicStateConfigurator::getType() const
{
    return typeid(DynamicStateConfigurator);
}
