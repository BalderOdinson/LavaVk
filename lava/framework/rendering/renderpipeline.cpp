//
// Created by dorian on 16. 12. 2019..
//

#include "renderpipeline.h"
#include "options/renderpipelineoptions.h"

LavaVk::RenderPipeline::RenderPipeline(LavaVk::InjectionContext &context)
{
    auto options = context.container.option<RenderPipelineOptions>(context);
    subpasses = options->subpasses;
    clearValue[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[1].depthStencil = vk::ClearDepthStencilValue{1.0f, ~0U};
}

LavaVk::RenderPipeline::RenderPipeline(const LavaVk::RenderPipelineOptions &options)
{
    subpasses = options.subpasses;
    clearValue[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[1].depthStencil = vk::ClearDepthStencilValue{1.0f, ~0U};
}

const std::vector<LavaVk::Core::LoadStoreInfo> &LavaVk::RenderPipeline::getLoadStore() const
{
    return loadStore;
}

void LavaVk::RenderPipeline::setLoadStore(const std::vector<Core::LoadStoreInfo> &ls)
{
    loadStore = ls;
}

const std::vector<vk::ClearValue> &LavaVk::RenderPipeline::getClearValue() const
{
    return clearValue;
}

void LavaVk::RenderPipeline::setClearValue(const std::vector<vk::ClearValue> &clearValues)
{
    clearValue = clearValues;
}

void LavaVk::RenderPipeline::addSubpass(LavaVk::SharedSubpass subpass)
{
    subpasses.emplace_back(std::move(subpass));
}

std::vector<LavaVk::SharedSubpass> &LavaVk::RenderPipeline::getSubpasses()
{
    return subpasses;
}

LavaVk::Core::BeginRenderPassToken LavaVk::RenderPipeline::draw(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                                const LavaVk::SharedRenderTarget &renderTarget)
{
    Core::BeginRenderPassToken token;
    for (size_t i = 0; i < subpasses.size(); ++i)
    {
        activeSubpassIndex = i;
        auto subpass = subpasses[i];
        subpass->updateRenderTargetAttachments();
        if (i == 0)
            token = commandBuffer->beginRenderPass(renderTarget, loadStore, clearValue, subpasses, subpass->getSubpassContents());
        else
            commandBuffer->nextSubpass(subpass->getSubpassContents());
        subpass->draw(commandBuffer);
    }

    activeSubpassIndex = 0;

    return token;
}

LavaVk::SharedSubpass LavaVk::RenderPipeline::getActiveSubpass()
{
    return subpasses[activeSubpassIndex];
}

std::type_index LavaVk::RenderPipeline::getType() const
{
    return typeid(RenderPipeline);
}

void LavaVk::RenderPipeline::prepare(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    for (auto &subpass : subpasses)
        subpass->prepare(commandBuffer);
}
