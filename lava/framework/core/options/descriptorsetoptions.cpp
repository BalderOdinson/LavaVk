//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorsetoptions.h"

#include <utility>

LavaVk::Core::DescriptorSetOptions::DescriptorSetOptions(LavaVk::Core::SharedDescriptorSetLayout descriptorSetLayout,
                                                         LavaVk::Core::SharedDescriptorPool descriptorPool,
                                                         LavaVk::BindingMap<vk::DescriptorBufferInfo> bufferInfos,
                                                         LavaVk::BindingMap<vk::BufferView> bufferViewsInfos,
                                                         LavaVk::BindingMap<vk::DescriptorImageInfo> imageInfos)
        : descriptorSetLayout(std::move(descriptorSetLayout)), descriptorPool(std::move(descriptorPool)),
          bufferInfos(std::move(bufferInfos)),
          bufferViewsInfos(std::move(bufferViewsInfos)), imageInfos(std::move(imageInfos))
{
}

std::type_index LavaVk::Core::DescriptorSetOptions::getType() const
{
    return typeid(DescriptorSetOptions);
}

bool LavaVk::Core::DescriptorSetOptions::equals(const LavaVk::Object &other) const
{
    auto dso = dynamic_cast<const DescriptorSetOptions *>(&other);
    if (dso)
    {
        return descriptorSetLayout->equals(*dso->descriptorSetLayout)
               && bufferInfos == dso->bufferInfos
               && bufferViewsInfos == dso->bufferViewsInfos
               && imageInfos == dso->imageInfos;
    }
    return Object::equals(other);
}

size_t LavaVk::Core::DescriptorSetOptions::getHashCode() const
{
    size_t result = 0;
    for (auto &binding : bufferInfos)
    {
        for (auto& element : binding.second)
        {
            size_t midResult = 0;
            glm::detail::hash_combine(midResult, std::hash<VkBuffer>()(element.second.buffer));
            glm::detail::hash_combine(midResult, std::hash<vk::DeviceSize>()(element.second.range));
            glm::detail::hash_combine(midResult, std::hash<vk::DeviceSize>()(element.second.offset));

            glm::detail::hash_combine(result, midResult);
        }
    }
    for (auto &binding : bufferViewsInfos)
    {
        for (auto& element : binding.second)
        {
            size_t midResult = 0;
            glm::detail::hash_combine(midResult, std::hash<VkBufferView >()(element.second));
            glm::detail::hash_combine(result, midResult);
        }
    }
    for (auto &binding : imageInfos)
    {
        for (auto& element : binding.second)
        {
            size_t midResult = 0;
            glm::detail::hash_combine(midResult, std::hash<uint32_t >()(static_cast<uint32_t>(element.second.imageLayout)));
            glm::detail::hash_combine(midResult, std::hash<VkImageView >()(element.second.imageView));
            glm::detail::hash_combine(midResult, std::hash<VkSampler>()(element.second.sampler));

            glm::detail::hash_combine(result, midResult);
        }
    }
    glm::detail::hash_combine(result, descriptorSetLayout->getHashCode());
    return result;
}
