//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorset.h"

#include <utility>
#include "descriptorpool.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/core/options/descriptorsetoptions.h"

LavaVk::Core::DescriptorSet::DescriptorSet(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    logger = context.container.resolve<Logger>();
    auto options = context.container.option<DescriptorSetOptions>(context);
    pool = options->descriptorPool;
    layout = options->descriptorSetLayout;
    handle = pool->allocate();
    bufferInfos = options->bufferInfos;
    bufferViewsInfos = options->bufferViewsInfos;
    imageInfos = options->imageInfos;

    if (!bufferInfos.empty() || !imageInfos.empty() || !bufferViewsInfos.empty())
        update(bufferInfos, bufferViewsInfos, imageInfos);
}

LavaVk::Core::DescriptorSet::~DescriptorSet()
{
    if (handle)
    {
        pool->free(handle);
        handle = nullptr;
    }
}

void LavaVk::Core::DescriptorSet::update(const LavaVk::BindingMap<vk::DescriptorBufferInfo> &buffInfos,
                                         const LavaVk::BindingMap<vk::BufferView> &buffViewsInfos,
                                         const LavaVk::BindingMap<vk::DescriptorImageInfo> &imgInfos)
{
    bufferInfos = buffInfos;
    bufferViewsInfos = buffViewsInfos;
    imageInfos = imgInfos;

    std::vector<vk::WriteDescriptorSet> setUpdates;

    // Iterate over all buffer bindings
    for (auto &bindingIt : bufferInfos)
    {
        auto binding = bindingIt.first;
        auto &bufferBindings = bindingIt.second;

        vk::DescriptorSetLayoutBinding bindingInfo{};
        if (!layout->tryGetLayoutBinding(binding, bindingInfo))
        {
            logger->error(std::string_format("Shader layout set does not use buffer binding at #%d", binding));
            continue;
        }

        // Iterate over all binding buffers in array
        for (auto &elementIt : bufferBindings)
        {
            auto arrayElement = elementIt.first;
            auto &bufferInfo = elementIt.second;

            vk::WriteDescriptorSet writeDescriptorSet{};

            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorType = bindingInfo.descriptorType;
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            writeDescriptorSet.dstSet = handle;
            writeDescriptorSet.dstArrayElement = arrayElement;
            writeDescriptorSet.descriptorCount = 1;

            setUpdates.push_back(writeDescriptorSet);
        }
    }

    // Iterate over all texel buffer bindings
    for (auto &bindingIt : bufferViewsInfos)
    {
        auto binding = bindingIt.first;
        auto &bufferBindings = bindingIt.second;

        vk::DescriptorSetLayoutBinding bindingInfo{};
        if (!layout->tryGetLayoutBinding(binding, bindingInfo))
        {
            logger->error(std::string_format("Shader layout set does not use texel buffer binding at #%d", binding));
            continue;
        }

        // Iterate over all binding buffers in array
        for (auto &elementIt : bufferBindings)
        {
            auto arrayElement = elementIt.first;
            auto &bufferInfo = elementIt.second;

            vk::WriteDescriptorSet writeDescriptorSet{};

            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorType = bindingInfo.descriptorType;
            writeDescriptorSet.pTexelBufferView = &bufferInfo;
            writeDescriptorSet.dstSet = handle;
            writeDescriptorSet.dstArrayElement = arrayElement;
            writeDescriptorSet.descriptorCount = 1;

            setUpdates.push_back(writeDescriptorSet);
        }
    }

    // Iterate over all image bindings
    for (auto &bindingIt : imageInfos)
    {
        auto bindingIndex = bindingIt.first;
        auto &bindingResources = bindingIt.second;

        vk::DescriptorSetLayoutBinding bindingInfo;
        if (!layout->tryGetLayoutBinding(bindingIndex, bindingInfo))
        {
            logger->error(std::string_format("Shader layout set does not use image binding at #%d", bindingIndex));
            continue;
        }

        // Iterate over all binding images in array
        for (auto &elementIt : bindingResources)
        {
            auto arrayElement = elementIt.first;
            auto &image_info = elementIt.second;

            vk::WriteDescriptorSet writeDescriptorSet{};

            writeDescriptorSet.dstBinding = bindingIndex;
            writeDescriptorSet.descriptorType = bindingInfo.descriptorType;
            writeDescriptorSet.pImageInfo = &image_info;
            writeDescriptorSet.dstSet = handle;
            writeDescriptorSet.dstArrayElement = arrayElement;
            writeDescriptorSet.descriptorCount = 1;

            setUpdates.push_back(writeDescriptorSet);
        }
    }

    device->getHandle().updateDescriptorSets(setUpdates, {});
}

const LavaVk::Core::SharedDescriptorSetLayout &LavaVk::Core::DescriptorSet::getLayout() const
{
    return layout;
}

vk::DescriptorSet LavaVk::Core::DescriptorSet::getHandle() const
{
    return handle;
}

LavaVk::BindingMap<vk::DescriptorBufferInfo> &LavaVk::Core::DescriptorSet::getBufferInfos()
{
    return bufferInfos;
}

LavaVk::BindingMap<vk::BufferView> &LavaVk::Core::DescriptorSet::getBufferViewInfos()
{
    return bufferViewsInfos;
}

LavaVk::BindingMap<vk::DescriptorImageInfo> &LavaVk::Core::DescriptorSet::getImageInfos()
{
    return imageInfos;
}

std::type_index LavaVk::Core::DescriptorSet::getType() const
{
    return typeid(DescriptorSet);
}
