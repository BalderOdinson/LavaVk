//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_PIPELINEBARRIER_H
#define LAVAVK_PIPELINEBARRIER_H

#include <vulkan/vulkan.hpp>
#include "commandbuffer.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk::Core
{
    class PipelineBarrier
    {
    public:
        explicit PipelineBarrier(InjectionContext &context);
        explicit PipelineBarrier(SharedCommandBuffer commandBuffer);

        PipelineBarrier &setDependencyFlags(const vk::DependencyFlags &flags);

        PipelineBarrier &
        addGlobalMemoryBarrier(const vk::AccessFlags &srcAccessFlags, const vk::AccessFlags &dstAccessFlags,
                               const vk::PipelineStageFlags &generating_stages,
                               const vk::PipelineStageFlags &consuming_stages);

        PipelineBarrier &addBufferMemoryBarrier(const vk::Buffer &buffer, const vk::AccessFlags &srcAccessFlags,
                                                       const vk::AccessFlags &dstAccessFlags,
                                                       const vk::PipelineStageFlags &generatingStages,
                                                       const vk::PipelineStageFlags &consumingStages,
                                                       uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex);
        PipelineBarrier &addBufferMemoryBarrier(const vk::Buffer &buffer, const vk::AccessFlags &srcAccessFlags,
                                                       const vk::AccessFlags &dstAccessFlags,
                                                       const vk::PipelineStageFlags &generatingStages,
                                                       const vk::PipelineStageFlags &consumingStages,
                                                       uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex,
                                                       vk::DeviceSize offset, vk::DeviceSize size);

        PipelineBarrier &addImageMemoryBarrier(const vk::Image &image, const vk::AccessFlags &srcAccessFlags,
                                                      const vk::AccessFlags &dstAccessFlags,
                                                      const vk::PipelineStageFlags &generatingStages,
                                                      const vk::PipelineStageFlags &consumingStages,
                                                      const vk::ImageLayout &currentImageLayout,
                                                      const vk::ImageLayout &newImageLayout,
                                                      uint32_t currentQueueFamilyIndex, uint32_t newQueueFamilyIndex,
                                                      const vk::ImageAspectFlags &aspect);

        void execute() const;

    private:
        SharedCommandBuffer commandBuffer;

        std::vector<vk::MemoryBarrier> memoryBarriers;
        std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers;
        std::vector<vk::ImageMemoryBarrier> imageMemoryBarriers;
        vk::DependencyFlags dependencyFlags{};
        vk::PipelineStageFlags generatingStages;
        vk::PipelineStageFlags consumingStages;

    };
}



#endif //LAVAVK_PIPELINEBARRIER_H
