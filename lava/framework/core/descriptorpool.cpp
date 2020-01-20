//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorpool.h"
#include "options/descriptorpooloptions.h"

LavaVk::Core::DescriptorPool::DescriptorPool(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<DescriptorPoolOptions>(context);
    descriptorSetLayout = options->descriptorSetLayout;

    const auto &bindings = descriptorSetLayout->getBindings();

    std::map<vk::DescriptorType, std::uint32_t> descriptorTypeCounts;

    // Count each type of descriptor set
    for (auto &binding : bindings)
        descriptorTypeCounts[binding.descriptorType] += binding.descriptorCount;

    // Allocate pool sizes array
    poolSizes.resize(descriptorTypeCounts.size());

    auto poolSizeIt = poolSizes.begin();

    // Fill pool size for each descriptor type count multiplied by the pool size
    for (auto &it : descriptorTypeCounts)
    {
        poolSizeIt->type = it.first;
        poolSizeIt->descriptorCount = it.second * options->poolSize;
        ++poolSizeIt;
    }

    poolMaxSets = options->poolSize;
}

LavaVk::Core::DescriptorPool::~DescriptorPool()
{
    // Destroy all descriptor pools
    for (auto &pool : pools)
    {
        if (pool)
        {
            device->getHandle().destroy(pool);
            pool = nullptr;
        }
    }
}

void LavaVk::Core::DescriptorPool::reset()
{
    // Reset all descriptor pools
    for (auto pool : pools)
        device->getHandle().resetDescriptorPool(pool);

    // Clear internal tracking of descriptor set allocations
    std::fill(poolSetsCount.begin(), poolSetsCount.end(), 0);
    setPoolMapping.clear();

    // Reset the pool index from which descriptor sets are allocated
    poolIndex = 0;
}

const LavaVk::Core::SharedDescriptorSetLayout &LavaVk::Core::DescriptorPool::getDescriptorSetLayout() const
{
    return descriptorSetLayout;
}

void LavaVk::Core::DescriptorPool::setDescriptorSetLayout(const LavaVk::Core::SharedDescriptorSetLayout &setLayout)
{
    descriptorSetLayout = setLayout;
}

vk::DescriptorSet LavaVk::Core::DescriptorPool::allocate()
{
    poolIndex = findAvailablePool(poolIndex);

    // Increment allocated set count for the current pool
    ++poolSetsCount[poolIndex];

    vk::DescriptorSetLayout setLayout = descriptorSetLayout->getHandle();

    vk::DescriptorSetAllocateInfo allocInfo{};

    allocInfo.descriptorPool = pools[poolIndex];
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &setLayout;

    // Allocate a new descriptor set from the current pool
    auto handle = device->getHandle().allocateDescriptorSets(allocInfo).front();

    // Store mapping between the descriptor set and the pool
    setPoolMapping.emplace(handle, poolIndex);

    return handle;
}

uint32_t LavaVk::Core::DescriptorPool::findAvailablePool(uint32_t searchIndex)
{
    // Create a new pool
    if (pools.size() <= searchIndex)
    {
        vk::DescriptorPoolCreateInfo createInfo{};

        createInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.poolSizeCount = static_cast<uint32_t >(poolSizes.size());
        createInfo.pPoolSizes = poolSizes.data();
        createInfo.maxSets = poolMaxSets;

        // Create the Vulkan descriptor pool
        auto handle = device->getHandle().createDescriptorPool(createInfo);

        // Store internally the Vulkan handle
        pools.push_back(handle);

        // Add set count for the descriptor pool
        poolSetsCount.push_back(0);

        return searchIndex;
    }
    else if (poolSetsCount[searchIndex] < poolMaxSets)
    {
        return searchIndex;
    }

    // Increment pool index
    return findAvailablePool(++searchIndex);
}

void LavaVk::Core::DescriptorPool::free(const vk::DescriptorSet &set)
{
    // Get the pool index of the descriptor set
    auto it = setPoolMapping.find(set);

    if (it == setPoolMapping.end())
        return;

    auto descPoolIndex = it->second;

    // Free descriptor set from the pool
    device->getHandle().freeDescriptorSets(pools[descPoolIndex], {set});

    // Remove descriptor set mapping to the pool
    setPoolMapping.erase(it);

    // Decrement allocated set count for the pool
    --poolSetsCount[descPoolIndex];

    // Change the current pool index to use the available pool
    poolIndex = descPoolIndex;
}

std::type_index LavaVk::Core::DescriptorPool::getType() const
{
    return typeid(DescriptorPool);
}
