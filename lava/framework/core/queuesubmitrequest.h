//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_QUEUESUBMITREQUEST_H
#define LAVAVK_QUEUESUBMITREQUEST_H

#include <vulkan/vulkan.hpp>

namespace LavaVk::Core
{
    class QueueSubmitRequest;

    class QueueSubmitRequest2
    {
    public:
        explicit QueueSubmitRequest2(QueueSubmitRequest &request);

        QueueSubmitRequest2 &addWaitSemaphore(vk::Semaphore waitSemaphore, vk::PipelineStageFlags waitStages);
        QueueSubmitRequest2 &addCommandBuffer(vk::CommandBuffer buffer);
        QueueSubmitRequest2 &addSignalSemaphore(vk::Semaphore signalSemaphore);
        QueueSubmitRequest &add();

    private:
        QueueSubmitRequest &request;
        std::vector<vk::Semaphore> ws;
        std::vector<vk::PipelineStageFlags> wp;
        std::vector<vk::CommandBuffer> bf;
        std::vector<vk::Semaphore> ss;
    };

    class QueueSubmitRequest
    {
    public:
        friend class QueueSubmitRequest2;

        explicit QueueSubmitRequest(vk::Queue queue);

        QueueSubmitRequest2 submitInfo();
        QueueSubmitRequest &setFence(vk::Fence fence);

        void submit();

    private:
        vk::Queue queue;
        std::vector<std::vector<vk::Semaphore>> ws;
        std::vector<std::vector<vk::PipelineStageFlags >> wp;
        std::vector<std::vector<vk::CommandBuffer >> bf;
        std::vector<std::vector<vk::Semaphore>> ss;
        vk::Fence fence{nullptr};

    };
}


#endif //LAVAVK_QUEUESUBMITREQUEST_H
