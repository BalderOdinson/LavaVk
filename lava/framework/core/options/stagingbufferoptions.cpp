//
// Created by dorian on 10. 12. 2019..
//

#include "stagingbufferoptions.h"

LavaVk::Core::StagingBufferOptions::StagingBufferOptions(vk::DeviceSize size, vk::DeviceSize n) : BufferOptions(size, n,
                                                                                                                vk::BufferUsageFlagBits::eTransferSrc,
                                                                                                                vma::MemoryUsage::eCpuToGpu,
                                                                                                                true)
{
}
