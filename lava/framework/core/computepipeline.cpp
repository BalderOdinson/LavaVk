//
// Created by dorian on 13. 12. 2019..
//

#include "computepipeline.h"
#include "options/pipelineoptions.h"

LavaVk::Core::ComputePipeline::ComputePipeline(LavaVk::InjectionContext &context) : Pipeline(context)
{
    auto options = context.container.option<PipelineOptions>(context);

    auto shaderModule = options->state.getPipelineLayout()->getStages().front();

    if (shaderModule->getStage() != vk::ShaderStageFlagBits::eCompute)
        throw std::runtime_error("Shader module stage is not compute!");

    vk::PipelineShaderStageCreateInfo stage{};

    stage.stage = shaderModule->getStage();
    stage.pName = shaderModule->getEntryPoint().c_str();

    auto module = shaderModule->getModule();
    stage.module = module.getHandle();

    // Create specialization info from tracked state.
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
    specializationInfo.mapEntryCount = static_cast<uint32_t >(mapEntries.size());
    specializationInfo.pMapEntries = mapEntries.data();
    specializationInfo.dataSize = data.size();
    specializationInfo.pData = data.data();

    stage.pSpecializationInfo = &specializationInfo;

    vk::ComputePipelineCreateInfo createInfo{};

    createInfo.layout = options->state.getPipelineLayout()->getHandle();
    createInfo.stage = stage;

    handle = device->getHandle().createComputePipeline(options->cache, createInfo);
}

std::type_index LavaVk::Core::ComputePipeline::getType() const
{
    return typeid(ComputePipeline);
}
