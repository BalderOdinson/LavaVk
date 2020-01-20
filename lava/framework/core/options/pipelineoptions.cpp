//
// Created by dorian on 13. 12. 2019..
//

#include "pipelineoptions.h"

LavaVk::Core::PipelineOptions::PipelineOptions(LavaVk::PipelineState &state, vk::PipelineCache cache) : state(state),
                                                                                                        cache(cache)
{
}

std::type_index LavaVk::Core::PipelineOptions::getType() const
{
    return typeid(PipelineOptions);
}

size_t LavaVk::Core::PipelineOptions::getHashCode() const
{
    std::size_t result = 0;

    glm::detail::hash_combine(result, state.getPipelineLayout()->getHashCode());

    // For graphics only
    if (auto renderPass = state.getRenderPass())
        glm::detail::hash_combine(result, renderPass->getHashCode());

    // SpecializationConstantState
    size_t midResult = 0;
    for (auto &constants : state.getSpecializationConstantState().getSpecializationConstantState())
    {
        glm::detail::hash_combine(midResult, constants.first);
        for (const auto data : constants.second)
            glm::detail::hash_combine(midResult, data);
    }
    glm::detail::hash_combine(result, midResult);

    glm::detail::hash_combine(result, state.getSubpassIndex());

    for (auto &stage : state.getPipelineLayout()->getStages())
        glm::detail::hash_combine(result, stage->getId());

    // VkPipelineVertexInputStateCreateInfo
    midResult = 0;
    for (auto &attribute : state.getVertexInputState().attributes)
    {
        glm::detail::hash_combine(midResult, attribute.binding);
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkFormat>::type>(attribute.format));
        glm::detail::hash_combine(midResult, attribute.location);
        glm::detail::hash_combine(midResult, attribute.offset);
    }
    glm::detail::hash_combine(result, midResult);

    midResult = 0;
    for (auto &binding : state.getVertexInputState().bindings)
    {
        glm::detail::hash_combine(midResult, binding.binding);
        glm::detail::hash_combine(midResult,
                                  static_cast<std::underlying_type<VkVertexInputRate>::type>(binding.inputRate));
        glm::detail::hash_combine(midResult, binding.stride);
    }
    glm::detail::hash_combine(result, midResult);

    // VkPipelineInputAssemblyStateCreateInfo
    glm::detail::hash_combine(result, state.getInputAssemblyState().primitiveRestartEnable);
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkPrimitiveTopology>::type>(state.getInputAssemblyState().topology));

    //VkPipelineViewportStateCreateInfo
    glm::detail::hash_combine(result, state.getViewportState().viewportCount);
    glm::detail::hash_combine(result, state.getViewportState().scissorCount);

    // VkPipelineRasterizationStateCreateInfo
    glm::detail::hash_combine(result, static_cast<VkCullModeFlags>(state.getRasterizationState().cullMode));
    glm::detail::hash_combine(result, state.getRasterizationState().depthBiasEnable);
    glm::detail::hash_combine(result, state.getRasterizationState().depthClampEnable);
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkFrontFace>::type>(state.getRasterizationState().frontFace));
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkPolygonMode>::type>(state.getRasterizationState().polygonMode));
    glm::detail::hash_combine(result, state.getRasterizationState().rasterizerDiscardEnable);

    // VkPipelineMultisampleStateCreateInfo
    glm::detail::hash_combine(result, state.getMultisampleState().alphaToCoverageEnable);
    glm::detail::hash_combine(result, state.getMultisampleState().alphaToOneEnable);
    glm::detail::hash_combine(result, state.getMultisampleState().minSampleShading);
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkSampleCountFlagBits>::type>(state.getMultisampleState().rasterizationSamples));
    glm::detail::hash_combine(result, state.getMultisampleState().sampleShadingEnable);
    glm::detail::hash_combine(result, state.getMultisampleState().sampleMask);

    // VkPipelineDepthStencilStateCreateInfo
    {
        midResult = 0;
        auto stencil = state.getDepthStencilState().back;
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkCompareOp>::type>(stencil.compareOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.depthFailOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.failOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.passOp));
    }
    glm::detail::hash_combine(result, midResult);
    glm::detail::hash_combine(result, state.getDepthStencilState().depthBoundsTestEnable);
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkCompareOp>::type>(state.getDepthStencilState().depthCompareOp));
    glm::detail::hash_combine(result, state.getDepthStencilState().depthTestEnable);
    glm::detail::hash_combine(result, state.getDepthStencilState().depthWriteEnable);
    {
        midResult = 0;
        auto stencil = state.getDepthStencilState().front;
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkCompareOp>::type>(stencil.compareOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.depthFailOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.failOp));
        glm::detail::hash_combine(midResult, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.passOp));
    }
    glm::detail::hash_combine(result, midResult);
    glm::detail::hash_combine(result, state.getDepthStencilState().stencilTestEnable);

    // VkPipelineColorBlendStateCreateInfo
    glm::detail::hash_combine(result,
                              static_cast<std::underlying_type<VkLogicOp>::type>(state.getColorBlendState().logicOp));
    glm::detail::hash_combine(result, state.getColorBlendState().logicOpEnable);

    midResult = 0;
    for (auto &attachment : state.getColorBlendState().attachments)
    {
        glm::detail::hash_combine(result, static_cast<std::underlying_type<VkBlendOp>::type>(attachment.alphaBlendOp));
        glm::detail::hash_combine(result, attachment.blendEnable);
        glm::detail::hash_combine(result, static_cast<std::underlying_type<VkBlendOp>::type>(attachment.colorBlendOp));
        glm::detail::hash_combine(result, static_cast<uint32_t >(attachment.colorWriteMask));
        glm::detail::hash_combine(result,
                                  static_cast<std::underlying_type<VkBlendFactor>::type>(attachment.dstAlphaBlendFactor));
        glm::detail::hash_combine(result,
                                  static_cast<std::underlying_type<VkBlendFactor>::type>(attachment.dstColorBlendFactor));
        glm::detail::hash_combine(result,
                                  static_cast<std::underlying_type<VkBlendFactor>::type>(attachment.srcAlphaBlendFactor));
        glm::detail::hash_combine(result,
                                  static_cast<std::underlying_type<VkBlendFactor>::type>(attachment.srcColorBlendFactor));
    }
    glm::detail::hash_combine(result, midResult);

    return result;
}
