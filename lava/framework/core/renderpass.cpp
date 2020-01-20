//
// Created by dorian on 12. 12. 2019..
//

#include "renderpass.h"
#include <algorithm>
#include <numeric>
#include "options/renderpassoptions.h"
#include "lava/framework/vkcommon.h"
#include "lava/framework/multisamplingoptions.h"

LavaVk::Core::RenderPass::RenderPass(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<RenderPassOptions>(context);
    subpassCount = std::max<size_t>(1, options->subpasses.size());
    inputAttachments.resize(subpassCount);
    colorAttachments.resize(subpassCount);
    depthStencilAttachments.resize(subpassCount);
    resolveAttachments.resize(subpassCount);
    auto samples = context.container.option<MultisamplingOptions>()->samples;
    std::unordered_map<uint32_t, uint32_t> outputResolveBinding;

    uint32_t depthStencilAttachment{VK_ATTACHMENT_UNUSED};

    std::vector<vk::AttachmentDescription> attachmentDescriptions;
    std::vector<uint32_t> ca;

    for (size_t i = 0, o = 0; i < options->attachments.size(); ++i)
    {
        vk::AttachmentDescription attachment{};

        attachment.format = options->attachments[i].format;
        attachment.samples = options->attachments[i].samples;
        attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        if (i < options->loadStoreInfos.size())
        {
            attachment.loadOp = options->loadStoreInfos[i].loadOp;
            attachment.storeOp = options->loadStoreInfos[i].storeOp;
            attachment.stencilLoadOp = options->loadStoreInfos[i].loadOp;
            attachment.stencilStoreOp = options->loadStoreInfos[i].storeOp;
        }
        else if (samples != vk::SampleCountFlagBits::e1 && attachment.samples == vk::SampleCountFlagBits::e1)
        {
            attachment.loadOp = vk::AttachmentLoadOp::eClear;
            attachment.storeOp = vk::AttachmentStoreOp::eStore;
            attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
            attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        }

        if (isDepthStencilFormat(attachment.format))
        {
            depthStencilAttachment = i;
            attachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        }
        else if (samples != vk::SampleCountFlagBits::e1 && attachment.samples != vk::SampleCountFlagBits::e1)
        {
            ca.push_back(i);
        }

        if (samples != vk::SampleCountFlagBits::e1
            && attachment.samples == vk::SampleCountFlagBits::e1)
            outputResolveBinding[ca[o++]] = i;

        attachmentDescriptions.push_back(std::move(attachment));
    }

    std::vector<vk::SubpassDescription> subpassDescriptions;
    subpassDescriptions.reserve(subpassCount);

    for (size_t i = 0; i < options->subpasses.size(); ++i)
    {
        auto &subpass = options->subpasses[i];

        // Fill color/depth attachments references
        for (auto oAttachment : subpass.outputAttachments)
            if (oAttachment != depthStencilAttachment)
            {
                colorAttachments[i].push_back({oAttachment, vk::ImageLayout::eColorAttachmentOptimal});
                if (samples != vk::SampleCountFlagBits::e1)
                    resolveAttachments[i].push_back({outputResolveBinding[oAttachment],
                                                     vk::ImageLayout::eColorAttachmentOptimal});
            }

        // Fill input attachments references
        for (auto iAttachment : subpass.inputAttachments)
        {
            if (isDepthStencilFormat(attachmentDescriptions[iAttachment].format))
                inputAttachments[i].push_back({iAttachment, vk::ImageLayout::eDepthStencilReadOnlyOptimal});

            else
            {
                if (samples != vk::SampleCountFlagBits::e1)
                    inputAttachments[i].push_back(
                            {outputResolveBinding[iAttachment], vk::ImageLayout::eShaderReadOnlyOptimal});
                else
                    inputAttachments[i].push_back({iAttachment, vk::ImageLayout::eShaderReadOnlyOptimal});
            }
        }

        if (depthStencilAttachment != VK_ATTACHMENT_UNUSED)
            depthStencilAttachments[i].push_back(
                    {depthStencilAttachment, vk::ImageLayout::eDepthStencilAttachmentOptimal});
    }

    for (size_t i = 0; i < options->subpasses.size(); ++i)
    {
        vk::SubpassDescription subpassDescription{};
        subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

        subpassDescription.pInputAttachments = inputAttachments[i].empty() ? nullptr : inputAttachments[i].data();
        subpassDescription.inputAttachmentCount = static_cast<uint32_t >(inputAttachments[i].size());

        subpassDescription.pColorAttachments = colorAttachments[i].empty() ? nullptr : colorAttachments[i].data();
        subpassDescription.colorAttachmentCount = static_cast<uint32_t >(colorAttachments[i].size());

        subpassDescription.pResolveAttachments = resolveAttachments[i].empty() ? nullptr : resolveAttachments[i].data();

        subpassDescription.pDepthStencilAttachment = depthStencilAttachments[i].empty() ? nullptr
                                                                                        : depthStencilAttachments[i].data();

        subpassDescriptions.push_back(subpassDescription);
    }

    // Default subpass
    if (options->subpasses.empty())
    {
        vk::SubpassDescription subpassDescription{};
        subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

        for (size_t k = 0; k < attachmentDescriptions.size(); ++k)
        {
            if (k == depthStencilAttachment)
                continue;

            if (samples != vk::SampleCountFlagBits::e1 &&
                attachmentDescriptions[k].samples == vk::SampleCountFlagBits::e1)
                resolveAttachments[0].push_back(
                        {static_cast<uint32_t>(k), vk::ImageLayout::eColorAttachmentOptimal});
            else
                colorAttachments[0].push_back({static_cast<uint32_t>(k), vk::ImageLayout::eGeneral});
        }

        subpassDescription.pColorAttachments = colorAttachments[0].data();
        subpassDescription.pResolveAttachments = resolveAttachments[0].empty() ? nullptr : resolveAttachments[0].data();

        if (depthStencilAttachment != VK_ATTACHMENT_UNUSED)
        {
            depthStencilAttachments[0].push_back(
                    {depthStencilAttachment, vk::ImageLayout::eDepthStencilAttachmentOptimal});

            subpassDescription.pDepthStencilAttachment = depthStencilAttachments[0].data();
        }

        subpassDescriptions.push_back(subpassDescription);
    }

    // Make the initial layout same as in the first subpass using that attachment
    for (auto &subpass : subpassDescriptions)
    {
        for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
        {
            auto reference = subpass.pColorAttachments[k];
            // Set it only if not defined yet
            if (attachmentDescriptions[reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                attachmentDescriptions[reference.attachment].initialLayout = reference.layout;
        }

        if (subpass.pResolveAttachments)
        {
            for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
            {
                auto reference = subpass.pResolveAttachments[k];
                // Set it only if not defined yet
                if (attachmentDescriptions[reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                    attachmentDescriptions[reference.attachment].initialLayout = reference.layout;
            }
        }

        for (uint32_t k = 0U; k < subpass.inputAttachmentCount; ++k)
        {
            auto reference = subpass.pInputAttachments[k];
            // Set it only if not defined yet
            if (attachmentDescriptions[reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                attachmentDescriptions[reference.attachment].initialLayout = reference.layout;
        }

        if (subpass.pDepthStencilAttachment)
        {
            auto reference = *subpass.pDepthStencilAttachment;
            // Set it only if not defined yet
            if (attachmentDescriptions[reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                attachmentDescriptions[reference.attachment].initialLayout = reference.layout;
        }
    }

    // Make the final layout same as the last subpass layout
    {
        auto &subpass = subpassDescriptions.back();

        for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
        {
            const auto &reference = subpass.pColorAttachments[k];
            attachmentDescriptions[reference.attachment].finalLayout = reference.layout;
        }

        if (subpass.pResolveAttachments)
        {
            for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
            {
                const auto &reference = subpass.pResolveAttachments[k];
                attachmentDescriptions[reference.attachment].finalLayout = reference.layout;
            }
        }

        for (uint32_t k = 0U; k < subpass.inputAttachmentCount; ++k)
        {
            const auto &reference = subpass.pInputAttachments[k];
            attachmentDescriptions[reference.attachment].finalLayout = reference.layout;

            // Do not use depth attachment if used as input
            if (reference.attachment == depthStencilAttachment)
                subpass.pDepthStencilAttachment = nullptr;
        }

        if (subpass.pDepthStencilAttachment)
        {
            const auto &reference = *subpass.pDepthStencilAttachment;
            attachmentDescriptions[reference.attachment].finalLayout = reference.layout;
        }
    }

    // Set subpass dependencies
    std::vector<vk::SubpassDependency> dependencies(subpassCount - 1);

    if (subpassCount > 1)
    {
        for (size_t i = 0; i < dependencies.size(); ++i)
        {
            // Transition input attachments from color attachment to shader read
            dependencies[i].srcSubpass = static_cast<uint32_t>(i);
            dependencies[i].dstSubpass = static_cast<uint32_t>(i) + 1u;
            dependencies[i].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            dependencies[i].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
            dependencies[i].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            dependencies[i].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;
            dependencies[i].dependencyFlags = vk::DependencyFlagBits::eByRegion;
        }
    }

    // Create render pass
    vk::RenderPassCreateInfo createInfo{};

    createInfo.attachmentCount = static_cast<uint32_t >(attachmentDescriptions.size());
    createInfo.pAttachments = attachmentDescriptions.data();
    createInfo.subpassCount = static_cast<uint32_t >(subpassCount);
    createInfo.pSubpasses = subpassDescriptions.data();
    createInfo.dependencyCount = static_cast<uint32_t >(dependencies.size());
    createInfo.pDependencies = dependencies.data();

    handle = device->getHandle().createRenderPass(createInfo);
}

LavaVk::Core::RenderPass::~RenderPass()
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::RenderPass LavaVk::Core::RenderPass::getHandle() const
{
    return handle;
}

uint32_t LavaVk::Core::RenderPass::getColorOutputCount(uint32_t subpassIndex) const
{
    return static_cast<uint32_t >(colorAttachments[subpassIndex].size());
}

std::type_index LavaVk::Core::RenderPass::getType() const
{
    return typeid(RenderPass);
}
