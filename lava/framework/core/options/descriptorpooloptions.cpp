//
// Created by dorian on 11. 12. 2019..
//

#include "descriptorpooloptions.h"

#include <utility>

LavaVk::Core::DescriptorPoolOptions::DescriptorPoolOptions(LavaVk::Core::SharedDescriptorSetLayout descriptorSetLayout,
                                                           uint32_t poolSize) : descriptorSetLayout(std::move(
        descriptorSetLayout)), poolSize(poolSize)
{
}

std::type_index LavaVk::Core::DescriptorPoolOptions::getType() const
{
    return typeid(DescriptorPoolOptions);
}
