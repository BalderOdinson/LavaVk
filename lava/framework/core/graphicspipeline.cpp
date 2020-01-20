//
// Created by dorian on 13. 12. 2019..
//

#include "graphicspipeline.h"
#include "options/pipelineoptions.h"

LavaVk::Core::GraphicsPipeline::GraphicsPipeline(LavaVk::InjectionContext &context) : Pipeline(context)
{
    auto options = context.container.option<PipelineOptions>(context);

    std::vector<ShaderModule::Module> shaderModules;

    std::vector<vk::PipelineShaderStageCreateInfo> stageCreateInfos;

    // Create specialization info from tracked state. This is shared by all shaders.
    std::vector<uint8_t> data{};
    std::vector<vk::SpecializationMapEntry> mapEntries{};

    const auto specializationConstantState = options->state.getSpecializationConstantState().getSpecializationConstantState();

    for (const auto &specializationConstant : specializationConstantState)
    {
        mapEntries.emplace_back(specializationConstant.first, static_cast<uint32_t >(data.size()),
                                specializationConstant.second.size());
        data.insert(data.end(), specializationConstant.second.begin(), specializationConstant.second.end());
    }

    vk::SpecializationInfo specializationInfo{};
    specializationInfo.mapEntryCount = static_cast<uint32_t>(mapEntries.size());
    specializationInfo.pMapEntries = mapEntries.data();
    specializationInfo.dataSize = data.size();
    specializationInfo.pData = data.data();

    for (auto &shaderModule : options->state.getPipelineLayout()->getStages())
    {
        vk::PipelineShaderStageCreateInfo stageCreateInfo{};

        stageCreateInfo.stage = shaderModule->getStage();
        stageCreateInfo.pName = shaderModule->getEntryPoint().c_str();

        // Create the Vulkan handle
        shaderModules.emplace_back(shaderModule->getModule());
        stageCreateInfo.module = shaderModules.back().getHandle();
        stageCreateInfo.pSpecializationInfo = &specializationInfo;
        stageCreateInfos.push_back(stageCreateInfo);
    }

    vk::GraphicsPipelineCreateInfo createInfo{};

    createInfo.stageCount = static_cast<uint32_t>(stageCreateInfos.size());
    createInfo.pStages = stageCreateInfos.data();

    vk::PipelineVertexInputStateCreateInfo vertexInputState{};

    vertexInputState.pVertexAttributeDescriptions = options->state.getVertexInputState().attributes.data();
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(
            options->state.getVertexInputState().attributes.size());

    vertexInputState.pVertexBindingDescriptions = options->state.getVertexInputState().bindings.data();
    vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(options->state.getVertexInputState().bindings.size());

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{};

    inputAssemblyState.topology = options->state.getInputAssemblyState().topology;
    inputAssemblyState.primitiveRestartEnable = options->state.getInputAssemblyState().primitiveRestartEnable;

    vk::PipelineViewportStateCreateInfo viewportState{};

    viewportState.viewportCount = options->state.getViewportState().viewportCount;
    viewportState.scissorCount = options->state.getViewportState().scissorCount;

    vk::PipelineRasterizationStateCreateInfo rasterizationState{};

    rasterizationState.depthClampEnable = options->state.getRasterizationState().depthClampEnable;
    rasterizationState.rasterizerDiscardEnable = options->state.getRasterizationState().rasterizerDiscardEnable;
    rasterizationState.polygonMode = options->state.getRasterizationState().polygonMode;
    rasterizationState.cullMode = options->state.getRasterizationState().cullMode;
    rasterizationState.frontFace = options->state.getRasterizationState().frontFace;
    rasterizationState.depthBiasEnable = options->state.getRasterizationState().depthBiasEnable;
    rasterizationState.depthBiasClamp = 1.0f;
    rasterizationState.depthBiasSlopeFactor = 1.0f;
    rasterizationState.lineWidth = 1.0f;

    vk::PipelineMultisampleStateCreateInfo multisampleState{};

    multisampleState.sampleShadingEnable = options->state.getMultisampleState().sampleShadingEnable;
    multisampleState.rasterizationSamples = options->state.getMultisampleState().rasterizationSamples;
    multisampleState.minSampleShading = options->state.getMultisampleState().minSampleShading;
    multisampleState.alphaToCoverageEnable = options->state.getMultisampleState().alphaToCoverageEnable;
    multisampleState.alphaToOneEnable = options->state.getMultisampleState().alphaToOneEnable;

    if (options->state.getMultisampleState().sampleMask)
        multisampleState.pSampleMask = &options->state.getMultisampleState().sampleMask;

    vk::PipelineDepthStencilStateCreateInfo depthStencilState{};

    depthStencilState.depthTestEnable = options->state.getDepthStencilState().depthTestEnable;
    depthStencilState.depthWriteEnable = options->state.getDepthStencilState().depthWriteEnable;
    depthStencilState.depthCompareOp = options->state.getDepthStencilState().depthCompareOp;
    depthStencilState.depthBoundsTestEnable = options->state.getDepthStencilState().depthBoundsTestEnable;
    depthStencilState.stencilTestEnable = options->state.getDepthStencilState().stencilTestEnable;
    depthStencilState.front.failOp = options->state.getDepthStencilState().front.failOp;
    depthStencilState.front.passOp = options->state.getDepthStencilState().front.passOp;
    depthStencilState.front.depthFailOp = options->state.getDepthStencilState().front.depthFailOp;
    depthStencilState.front.compareOp = options->state.getDepthStencilState().front.compareOp;
    depthStencilState.front.compareMask = ~0U;
    depthStencilState.front.writeMask = ~0U;
    depthStencilState.front.reference = ~0U;
    depthStencilState.back.failOp = options->state.getDepthStencilState().back.failOp;
    depthStencilState.back.passOp = options->state.getDepthStencilState().back.passOp;
    depthStencilState.back.depthFailOp = options->state.getDepthStencilState().back.depthFailOp;
    depthStencilState.back.compareOp = options->state.getDepthStencilState().back.compareOp;
    depthStencilState.back.compareMask = ~0U;
    depthStencilState.back.writeMask = ~0U;
    depthStencilState.back.reference = ~0U;

    vk::PipelineColorBlendStateCreateInfo colorBlendState{};

    colorBlendState.logicOpEnable = options->state.getColorBlendState().logicOpEnable;
    colorBlendState.logicOp = options->state.getColorBlendState().logicOp;
    colorBlendState.attachmentCount = static_cast<uint32_t>(options->state.getColorBlendState().attachments.size());
    colorBlendState.pAttachments = reinterpret_cast<const vk::PipelineColorBlendAttachmentState *>(options->state.getColorBlendState().attachments.data());
    colorBlendState.blendConstants[0] = 1.0f;
    colorBlendState.blendConstants[1] = 1.0f;
    colorBlendState.blendConstants[2] = 1.0f;
    colorBlendState.blendConstants[3] = 1.0f;

    std::array<vk::DynamicState, 9> dynamicStates{
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
            vk::DynamicState::eLineWidth,
            vk::DynamicState::eDepthBias,
            vk::DynamicState::eBlendConstants,
            vk::DynamicState::eDepthBounds,
            vk::DynamicState::eStencilCompareMask,
            vk::DynamicState::eStencilWriteMask,
            vk::DynamicState::eStencilReference,
    };

    vk::PipelineDynamicStateCreateInfo dynamicState{};

    dynamicState.pDynamicStates = dynamicStates.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());

    createInfo.pVertexInputState = &vertexInputState;
    createInfo.pInputAssemblyState = &inputAssemblyState;
    createInfo.pViewportState = &viewportState;
    createInfo.pRasterizationState = &rasterizationState;
    createInfo.pMultisampleState = &multisampleState;
    createInfo.pDepthStencilState = &depthStencilState;
    createInfo.pColorBlendState = &colorBlendState;
    createInfo.pDynamicState = &dynamicState;

    createInfo.layout = options->state.getPipelineLayout()->getHandle();
    createInfo.renderPass = options->state.getRenderPass()->getHandle();
    createInfo.subpass = options->state.getSubpassIndex();

    handle = device->getHandle().createGraphicsPipeline(options->cache, createInfo);

    state = options->state;
}

std::type_index LavaVk::Core::GraphicsPipeline::getType() const
{
    return typeid(GraphicsPipeline);
}
