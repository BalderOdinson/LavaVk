//
// Created by dorian on 13. 12. 2019..
//

#include "pipelinestate.h"

namespace LavaVk
{
    bool operator==(const vk::VertexInputAttributeDescription &lhs, const vk::VertexInputAttributeDescription &rhs)
    {
        return std::tie(lhs.binding, lhs.format, lhs.location, lhs.offset) ==
               std::tie(rhs.binding, rhs.format, rhs.location, rhs.offset);
    }

    bool operator==(const vk::VertexInputBindingDescription &lhs, const vk::VertexInputBindingDescription &rhs)
    {
        return std::tie(lhs.binding, lhs.inputRate, lhs.stride) == std::tie(rhs.binding, rhs.inputRate, rhs.stride);
    }

    bool operator==(const LavaVk::ColorBlendAttachmentState &lhs, const LavaVk::ColorBlendAttachmentState &rhs)
    {
        return std::tie(lhs.alphaBlendOp, lhs.blendEnable, lhs.colorBlendOp, lhs.colorWriteMask,
                        lhs.dstAlphaBlendFactor,
                        lhs.dstColorBlendFactor, lhs.srcAlphaBlendFactor, lhs.srcColorBlendFactor) ==
               std::tie(rhs.alphaBlendOp, rhs.blendEnable, rhs.colorBlendOp, rhs.colorWriteMask,
                        rhs.dstAlphaBlendFactor,
                        rhs.dstColorBlendFactor, rhs.srcAlphaBlendFactor, rhs.srcColorBlendFactor);
    }

    bool operator!=(const LavaVk::StencilOpState &lhs, const LavaVk::StencilOpState &rhs)
    {
        return std::tie(lhs.compareOp, lhs.depthFailOp, lhs.failOp, lhs.passOp) !=
               std::tie(rhs.compareOp, rhs.depthFailOp, rhs.failOp, rhs.passOp);
    }

    bool operator!=(const LavaVk::VertexInputState &lhs, const LavaVk::VertexInputState &rhs)
    {
        return lhs.attributes != rhs.attributes || lhs.bindings != rhs.bindings;
    }

    bool operator!=(const LavaVk::InputAssemblyState &lhs, const LavaVk::InputAssemblyState &rhs)
    {
        return std::tie(lhs.primitiveRestartEnable, lhs.topology) != std::tie(rhs.primitiveRestartEnable, rhs.topology);
    }

    bool operator!=(const LavaVk::RasterizationState &lhs, const LavaVk::RasterizationState &rhs)
    {
        return std::tie(lhs.cullMode, lhs.depthBiasEnable, lhs.depthClampEnable, lhs.frontFace, lhs.frontFace,
                        lhs.polygonMode, lhs.rasterizerDiscardEnable) !=
               std::tie(rhs.cullMode, rhs.depthBiasEnable, rhs.depthClampEnable, rhs.frontFace, rhs.frontFace,
                        rhs.polygonMode, rhs.rasterizerDiscardEnable);
    }

    bool operator!=(const LavaVk::ViewportState &lhs, const LavaVk::ViewportState &rhs)
    {
        return lhs.viewportCount != rhs.viewportCount || lhs.scissorCount != rhs.scissorCount;
    }

    bool operator!=(const LavaVk::MultisampleState &lhs, const LavaVk::MultisampleState &rhs)
    {
        return std::tie(lhs.alphaToCoverageEnable, lhs.alphaToOneEnable, lhs.minSampleShading,
                        lhs.rasterizationSamples, lhs.sampleMask, lhs.sampleShadingEnable) !=
               std::tie(rhs.alphaToCoverageEnable, rhs.alphaToOneEnable, rhs.minSampleShading, rhs.rasterizationSamples,
                        rhs.sampleMask, rhs.sampleShadingEnable);
    }

    bool operator!=(const LavaVk::DepthStencilState &lhs, const LavaVk::DepthStencilState &rhs)
    {
        return std::tie(lhs.depthBoundsTestEnable, lhs.depthCompareOp, lhs.depthTestEnable, lhs.depthWriteEnable,
                        lhs.stencilTestEnable) !=
               std::tie(rhs.depthBoundsTestEnable, rhs.depthCompareOp, rhs.depthTestEnable, rhs.depthWriteEnable,
                        rhs.stencilTestEnable) ||
               lhs.back != rhs.back || lhs.front != rhs.front;
    }

    bool operator!=(const LavaVk::ColorBlendState &lhs, const LavaVk::ColorBlendState &rhs)
    {
        return std::tie(lhs.logicOp, lhs.logicOpEnable) != std::tie(rhs.logicOp, rhs.logicOpEnable) ||
               lhs.attachments.size() != rhs.attachments.size() ||
               !std::equal(lhs.attachments.begin(), lhs.attachments.end(), rhs.attachments.begin(),
                           [](const LavaVk::ColorBlendAttachmentState &lhs,
                              const LavaVk::ColorBlendAttachmentState &rhs)
                           {
                               return lhs == rhs;
                           });
    }
}

void LavaVk::SpecializationConstantState::reset()
{
    if (dirty)
        specializationConstantState.clear();

    dirty = false;
}

bool LavaVk::SpecializationConstantState::isDirty() const
{
    return dirty;
}

void LavaVk::SpecializationConstantState::clearDirty()
{
    dirty = false;
}

void LavaVk::SpecializationConstantState::setConstant(uint32_t constantId, const std::vector<uint8_t> &value)
{
    auto data = specializationConstantState.find(constantId);

    if (data != specializationConstantState.end() && data->second == value)
        return;

    dirty = true;

    specializationConstantState[constantId] = value;
}

void LavaVk::SpecializationConstantState::setSpecializationConstantState(
        const std::map<uint32_t, std::vector<uint8_t>> &state)
{
    specializationConstantState = state;
}

const std::map<uint32_t, std::vector<uint8_t>> &
LavaVk::SpecializationConstantState::getSpecializationConstantState() const
{
    return specializationConstantState;
}

void LavaVk::PipelineState::reset()
{
    clearDirty();
    pipelineLayout = nullptr;
    renderPass = nullptr;
    specializationConstantState.reset();
    vertexInputSate = {};
    inputAssemblyState = {};
    rasterizationState = {};
    multisampleState = {};
    depthStencilState = {};
    colorBlendState = {};
    subpassIndex = {0U};
}

void LavaVk::PipelineState::setPipelineLayout(LavaVk::Core::SharedPipelineLayout newPipelineLayout)
{
    if (pipelineLayout)
    {
        if (!pipelineLayout->equals(*newPipelineLayout))
        {
            pipelineLayout = newPipelineLayout;
            dirty = true;
        }
    }
    else
    {
        pipelineLayout = newPipelineLayout;
        dirty = true;
    }
}

void LavaVk::PipelineState::setRenderPass(LavaVk::Core::SharedRenderPass newRenderPass)
{
    if (renderPass)
    {
        if (!renderPass->equals(*newRenderPass))
        {
            renderPass = newRenderPass;
            dirty = true;
        }
    }
    else
    {
        renderPass = newRenderPass;
        dirty = true;
    }
}

void LavaVk::PipelineState::setSpecializationConstant(uint32_t constantId, const std::vector<uint8_t> &data)
{
    specializationConstantState.setConstant(constantId, data);

    if (specializationConstantState.isDirty())
        dirty = true;
}

void LavaVk::PipelineState::setVertexInputState(const LavaVk::VertexInputState &newVertexInputSate)
{
    if (vertexInputSate != newVertexInputSate)
    {
        vertexInputSate = newVertexInputSate;
        dirty = true;
    }
}

void LavaVk::PipelineState::setInputAssemblyState(const LavaVk::InputAssemblyState &newInputAssemblyState)
{
    if (inputAssemblyState != newInputAssemblyState)
    {
        inputAssemblyState = newInputAssemblyState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setRasterizationState(const LavaVk::RasterizationState &newRasterizationState)
{
    if (rasterizationState != newRasterizationState)
    {
        rasterizationState = newRasterizationState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setViewportState(const LavaVk::ViewportState &newViewportState)
{
    if (viewportState != newViewportState)
    {
        viewportState = newViewportState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setMultisampleState(const LavaVk::MultisampleState &newMultisampleState)
{
    if (multisampleState != newMultisampleState)
    {
        multisampleState = newMultisampleState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setDepthStencilState(const LavaVk::DepthStencilState &newDepthStencilState)
{
    if (depthStencilState != newDepthStencilState)
    {
        depthStencilState = newDepthStencilState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setColorBlendState(const LavaVk::ColorBlendState &newColorBlendState)
{
    if (colorBlendState != newColorBlendState)
    {
        colorBlendState = newColorBlendState;
        dirty = true;
    }
}

void LavaVk::PipelineState::setSubpassIndex(uint32_t newSubpassIndex)
{
    if (subpassIndex != newSubpassIndex)
    {
        subpassIndex = newSubpassIndex;
        dirty = true;
    }
}

LavaVk::Core::SharedPipelineLayout LavaVk::PipelineState::getPipelineLayout() const
{
    return pipelineLayout;
}

LavaVk::Core::SharedRenderPass LavaVk::PipelineState::getRenderPass() const
{
    return renderPass;
}

const LavaVk::SpecializationConstantState &LavaVk::PipelineState::getSpecializationConstantState() const
{
    return specializationConstantState;
}

const LavaVk::VertexInputState &LavaVk::PipelineState::getVertexInputState() const
{
    return vertexInputSate;
}

const LavaVk::InputAssemblyState &LavaVk::PipelineState::getInputAssemblyState() const
{
    return inputAssemblyState;
}

const LavaVk::RasterizationState &LavaVk::PipelineState::getRasterizationState() const
{
    return rasterizationState;
}

const LavaVk::ViewportState &LavaVk::PipelineState::getViewportState() const
{
    return viewportState;
}

const LavaVk::MultisampleState &LavaVk::PipelineState::getMultisampleState() const
{
    return multisampleState;
}

const LavaVk::DepthStencilState &LavaVk::PipelineState::getDepthStencilState() const
{
    return depthStencilState;
}

const LavaVk::ColorBlendState &LavaVk::PipelineState::getColorBlendState() const
{
    return colorBlendState;
}

uint32_t LavaVk::PipelineState::getSubpassIndex() const
{
    return subpassIndex;
}

bool LavaVk::PipelineState::isDirty() const
{
    return dirty || specializationConstantState.isDirty();
}

void LavaVk::PipelineState::clearDirty()
{
    dirty = false;
    specializationConstantState.clearDirty();
}
