//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorsetlayout.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/platform/application.h"

inline vk::DescriptorType findDescriptorType(LavaVk::Core::ShaderResourceType resourceType, bool dynamic)
{
    switch (resourceType)
    {
        case LavaVk::Core::ShaderResourceType::InputAttachment:
            return vk::DescriptorType::eInputAttachment;
        case LavaVk::Core::ShaderResourceType::Image:
            return vk::DescriptorType::eSampledImage;
        case LavaVk::Core::ShaderResourceType::ImageSampler:
            return vk::DescriptorType::eCombinedImageSampler;
        case LavaVk::Core::ShaderResourceType::ImageStorage:
            return vk::DescriptorType::eStorageImage;
        case LavaVk::Core::ShaderResourceType::Sampler:
            return vk::DescriptorType::eSampler;
        case LavaVk::Core::ShaderResourceType::BufferUniform:
            if (dynamic)
                return vk::DescriptorType::eUniformBufferDynamic;
            else
                return vk::DescriptorType::eUniformBuffer;
        case LavaVk::Core::ShaderResourceType::BufferStorage:
            if (dynamic)
                return vk::DescriptorType::eStorageBufferDynamic;
            else
                return vk::DescriptorType::eStorageBuffer;
        case LavaVk::Core::ShaderResourceType::BufferUniformTexel:
            return vk::DescriptorType::eUniformTexelBuffer;
        case LavaVk::Core::ShaderResourceType::BufferStorageTexel:
            return vk::DescriptorType::eStorageTexelBuffer;
        default:
            throw std::runtime_error("No conversion possible for the shader resource type.");
    }
}

LavaVk::Core::DescriptorSetLayout::DescriptorSetLayout(InjectionContext &context) : DescriptorSetLayout(
        *context.container.option<DescriptorSetLayoutOptions>(context))
{
}

LavaVk::Core::DescriptorSetLayout::DescriptorSetLayout(const LavaVk::Core::DescriptorSetLayoutOptions &options)
{
    device = Application::instance->container.resolve<Device>();

    for (auto &resource : options.setResources)
    {
        // Skip shader resources whitout a binding point
        if (resource.type == ShaderResourceType::Input ||
            resource.type == ShaderResourceType::Output ||
            resource.type == ShaderResourceType::PushConstant ||
            resource.type == ShaderResourceType::SpecializationConstant)
        {
            continue;
        }

        // Convert from ShaderResourceType to VkDescriptorType.
        auto descriptor_type = findDescriptorType(resource.type, resource.dynamic);

        // Convert ShaderResource to vk::DescriptorSetLayoutBinding
        vk::DescriptorSetLayoutBinding layoutBinding{};

        layoutBinding.binding = resource.binding;
        layoutBinding.descriptorCount = resource.arraySize;
        layoutBinding.descriptorType = descriptor_type;
        layoutBinding.stageFlags = resource.stages;

        bindings.push_back(layoutBinding);

        // Store mapping between binding and the binding point
        bindingsLookup.emplace(resource.binding, layoutBinding);

        resourcesLookup.emplace(resource.name, resource.binding);
    }

    vk::DescriptorSetLayoutCreateInfo createInfo{};

    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    createInfo.pBindings = bindings.data();

    handle = device->getHandle().createDescriptorSetLayout(createInfo);
}

LavaVk::Core::DescriptorSetLayout::~DescriptorSetLayout()
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::DescriptorSetLayout LavaVk::Core::DescriptorSetLayout::getHandle() const
{
    return handle;
}

const std::vector<vk::DescriptorSetLayoutBinding> &LavaVk::Core::DescriptorSetLayout::getBindings() const
{
    return bindings;
}

bool LavaVk::Core::DescriptorSetLayout::tryGetLayoutBinding(uint32_t bindingIndex,
                                                            vk::DescriptorSetLayoutBinding &binding) const
{
    auto it = bindingsLookup.find(bindingIndex);
    if (it == bindingsLookup.end())
        return false;
    binding = it->second;
    return true;
}

bool LavaVk::Core::DescriptorSetLayout::tryGetLayoutBinding(const std::string &name,
                                                            vk::DescriptorSetLayoutBinding &binding) const
{
    return resourcesLookup.find(name) != resourcesLookup.end() &&
           tryGetLayoutBinding(resourcesLookup.at(name), binding);
}

std::type_index LavaVk::Core::DescriptorSetLayout::getType() const
{
    return typeid(DescriptorSetLayout);
}


