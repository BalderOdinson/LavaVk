//
// Created by oshikuru on 27. 12. 2019..
//

#include "multisamplingoptions.h"

LavaVk::MultisamplingOptions::MultisamplingOptions(vk::SampleCountFlagBits samples) : samples(samples)
{
}

std::type_index LavaVk::MultisamplingOptions::getType() const
{
    return typeid(MultisamplingOptions);
}
