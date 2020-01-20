//
// Created by dorian on 12. 12. 2019..
//

#include "pipelinelayout.h"
#include "options/pipelinelayoutoptions.h"
#include "lava/framework/resourcecache.h"

LavaVk::Core::PipelineLayout::PipelineLayout(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<PipelineLayoutOptions>(context);
    shaderModules = options->shaderModules;
    auto cache = context.container.resolve<ResourceCache>();

    // Merge shader stages resources
    for (auto &stage : shaderModules)
    {
        // Iterate over all of the shader resources
        for (const ShaderResource &resource : stage->getResources())
        {
            std::string key = resource.name;

            // Update name as input and output resources can have the same name
            if (resource.type == ShaderResourceType::Output || resource.type == ShaderResourceType::Input)
                key = std::string_format("%d_%s", static_cast<uint32_t >(resource.stages), key.c_str());

            // Find resource by name in the map
            auto it = resources.find(key);

            if (it != resources.end())
            {
                // Append stage flags if resource already exists
                it->second.stages |= resource.stages;
            }
            else
            {
                // Create a new entry in the map
                resources.emplace(key, resource);
            }
        }
    }

    // Separate all resources by set index
    for (auto &it : resources)
    {
        ShaderResource &resource = it.second;

        // Find binding by set index in the map.
        auto it2 = setBindings.find(resource.set);

        if (it2 != setBindings.end())
        {
            // Add resource to the found set index
            it2->second.push_back(resource);
        }
        else
        {
            // Create a new set index and with the first resource
            setBindings.emplace(resource.set, std::vector<ShaderResource>{resource});
        }
    }

    // Create a descriptor set layout for each set index
    for (auto &it : setBindings)
    {
        size_t result;
        glm::detail::hash_combine(result, it.second[0].arraySize);
        setLayouts.emplace(it.first, cache->requestDescriptorSetLayout(make<DescriptorSetLayoutOptions>(it.second)));
    }

    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

    for (auto &it : setLayouts)
        descriptorSetLayouts.push_back(it.second->getHandle());

    std::vector<vk::PushConstantRange> pushConstantRanges;

    for (auto &it : resources)
    {
        ShaderResource &resource = it.second;

        if (resource.type != ShaderResourceType::PushConstant)
            continue;

        vk::PushConstantRange range{};

        range.stageFlags = resource.stages;
        range.offset = resource.offset;
        range.size = resource.size;

        pushConstantRanges.push_back(range);
    }

    vk::PipelineLayoutCreateInfo createInfo{};

    createInfo.setLayoutCount = static_cast<uint32_t >(descriptorSetLayouts.size());
    createInfo.pSetLayouts = descriptorSetLayouts.data();
    createInfo.pushConstantRangeCount = static_cast<uint32_t >(pushConstantRanges.size());
    createInfo.pPushConstantRanges = pushConstantRanges.data();

    // Create the Vulkan pipeline layout handle
    handle = device->getHandle().createPipelineLayout(createInfo);
}

LavaVk::Core::PipelineLayout::~PipelineLayout()
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::PipelineLayout LavaVk::Core::PipelineLayout::getHandle()
{
    return handle;
}

const std::vector<LavaVk::Core::SharedShaderModule> &LavaVk::Core::PipelineLayout::getStages() const
{
    return shaderModules;
}

const std::unordered_map<uint32_t, std::vector<LavaVk::Core::ShaderResource>> &
LavaVk::Core::PipelineLayout::getBindings() const
{
    return setBindings;
}

const std::vector<LavaVk::Core::ShaderResource> &LavaVk::Core::PipelineLayout::getSetBindings(uint32_t setIndex) const
{
    return setBindings.at(setIndex);
}

bool LavaVk::Core::PipelineLayout::hasSetLayout(uint32_t setIndex)
{
    return setIndex < setLayouts.size();
}

LavaVk::Core::SharedDescriptorSetLayout LavaVk::Core::PipelineLayout::getSetLayout(uint32_t setIndex)
{
    return setLayouts[setIndex];
}

std::vector<LavaVk::Core::ShaderResource> LavaVk::Core::PipelineLayout::getVertexInputAttributes()
{
    std::vector<ShaderResource> vertexInputAttributes;

    // Iterate over all resources
    for (auto &it : resources)
        if (it.second.stages == vk::ShaderStageFlagBits::eVertex &&
            it.second.type == ShaderResourceType::Input)
            vertexInputAttributes.push_back(it.second);

    return vertexInputAttributes;
}

std::vector<LavaVk::Core::ShaderResource> LavaVk::Core::PipelineLayout::getFragmentOutputAttachments() const
{
    std::vector<ShaderResource> fragmentOutputAttachments;

    // Iterate over all resources
    for (auto &it : resources)
        if (it.second.stages == vk::ShaderStageFlagBits::eFragment &&
            it.second.type == ShaderResourceType::Output)
            fragmentOutputAttachments.push_back(it.second);

    return fragmentOutputAttachments;
}

std::vector<LavaVk::Core::ShaderResource> LavaVk::Core::PipelineLayout::getFragmentInputAttachments() const
{
    std::vector<ShaderResource> fragmentInputAttachments;

    // Iterate over all resources
    for (auto &it : resources)
        if (it.second.stages == vk::ShaderStageFlagBits::eFragment  &&
            it.second.type == ShaderResourceType::InputAttachment)
            fragmentInputAttachments.push_back(it.second);

    return fragmentInputAttachments;
}

vk::ShaderStageFlags
LavaVk::Core::PipelineLayout::getPushConstantRangeStage(uint32_t offset, uint32_t size)
{
    vk::ShaderStageFlags stages{};

    // Iterate over all resources
    for (auto &it : resources)
        if (it.second.type == ShaderResourceType::PushConstant)
            if (offset >= it.second.offset && offset + size <= it.second.offset + it.second.size)
                stages |= it.second.stages;
    return stages;
}

std::type_index LavaVk::Core::PipelineLayout::getType() const
{
    return typeid(PipelineLayout);
}
