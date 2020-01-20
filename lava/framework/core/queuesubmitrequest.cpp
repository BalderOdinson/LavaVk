//
// Created by dorian on 08. 12. 2019..
//

#include "queuesubmitrequest.h"
#include "queue.h"

LavaVk::Core::QueueSubmitRequest2::QueueSubmitRequest2(LavaVk::Core::QueueSubmitRequest &request) : request(request)
{
}

LavaVk::Core::QueueSubmitRequest2 &LavaVk::Core::QueueSubmitRequest2::addWaitSemaphore(vk::Semaphore waitSemaphore, vk::PipelineStageFlags waitStages)
{
    ws.push_back(waitSemaphore);
    wp.push_back(waitStages);
    return *this;
}

LavaVk::Core::QueueSubmitRequest2 &LavaVk::Core::QueueSubmitRequest2::addCommandBuffer(vk::CommandBuffer buffer)
{
    bf.push_back(buffer);
    return *this;
}

LavaVk::Core::QueueSubmitRequest2 &LavaVk::Core::QueueSubmitRequest2::addSignalSemaphore(vk::Semaphore signalSemaphore)
{
    ss.push_back(signalSemaphore);
    return *this;
}

LavaVk::Core::QueueSubmitRequest &LavaVk::Core::QueueSubmitRequest2::add()
{
    request.ws.emplace_back(std::move(ws));
    request.wp.emplace_back(std::move(wp));
    request.bf.emplace_back(std::move(bf));
    request.ss.emplace_back(std::move(ss));
    return request;
}

LavaVk::Core::QueueSubmitRequest::QueueSubmitRequest(vk::Queue queue) : queue(queue)
{
}

LavaVk::Core::QueueSubmitRequest2 LavaVk::Core::QueueSubmitRequest::submitInfo()
{
    return LavaVk::Core::QueueSubmitRequest2(*this);
}

LavaVk::Core::QueueSubmitRequest &LavaVk::Core::QueueSubmitRequest::setFence(vk::Fence newFence)
{
    fence = newFence;
    return *this;
}

void LavaVk::Core::QueueSubmitRequest::submit()
{
    std::vector<vk::SubmitInfo> submitInfos(ws.size());
    if(submitInfos.empty())
        return;

    for (size_t i = 0; i < submitInfos.size(); ++i)
    {
        submitInfos[i].waitSemaphoreCount = ws[i].size();
        submitInfos[i].pWaitSemaphores = ws[i].data();
        submitInfos[i].pWaitDstStageMask = wp[i].data();
        submitInfos[i].commandBufferCount = bf[i].size();
        submitInfos[i].pCommandBuffers = bf[i].data();
        submitInfos[i].signalSemaphoreCount = ss[i].size();
        submitInfos[i].pSignalSemaphores = ss[i].data();
    }

    queue.submit(submitInfos, fence);

}
