//
// Created by dorian on 16. 12. 2019..
//

#include "subpass.h"

#include <utility>
#include "options/subpassoptions.h"
#include "lava/framework/resourcecache.h"

LavaVk::Subpass::Subpass(LavaVk::InjectionContext &context)
{
    resourceCache = context.container.resolve<ResourceCache>();
    renderContext = context.container.resolve<RenderContext>();
    auto options = context.container.option<SubpassOptions>(context);
}

void LavaVk::Subpass::updateRenderTargetAttachments()
{
    auto renderTarget = renderContext->getActiveFrame()->getRenderTarget();
    renderTarget->setInputAttachments(inputAttachments);
    renderTarget->setOutputAttachments(outputAttachments);
    renderTarget->setResolveAttachments(resolveAttachments);
}

LavaVk::SharedRenderContext LavaVk::Subpass::getRenderContext() const
{
    return renderContext;
}

LavaVk::DepthStencilState &LavaVk::Subpass::getDepthStencilState()
{
    return depthStencilState;
}

const std::vector<uint32_t> &LavaVk::Subpass::getInputAttachments() const
{
    return inputAttachments;
}

void LavaVk::Subpass::setInputAttachments(std::vector<uint32_t> input)
{
    inputAttachments = std::move(input);
}

const std::vector<uint32_t> &LavaVk::Subpass::getOutputAttachments() const
{
    return outputAttachments;
}

void LavaVk::Subpass::setOutputAttachments(std::vector<uint32_t> output)
{
    outputAttachments = std::move(output);
}

const std::vector<uint32_t> &LavaVk::Subpass::getResolveAttachments() const
{
    return resolveAttachments;
}

void LavaVk::Subpass::setResolveAttachments(std::vector<uint32_t> resolve)
{
    resolveAttachments = std::move(resolve);
}

