//
// Created by dorian on 08. 12. 2019..
//

#include "queuepresentrequest.h"

LavaVk::Core::QueuePresentRequest::QueuePresentRequest(vk::Queue queue) : queue(queue)
{
}

LavaVk::Core::QueuePresentRequest &
LavaVk::Core::QueuePresentRequest::addSwapchain(vk::SwapchainKHR swapchain, uint32_t imageIndex)
{
    swapchains.push_back(swapchain);
    imageIndices.push_back(imageIndex);
    return *this;
}

LavaVk::Core::QueuePresentRequest &LavaVk::Core::QueuePresentRequest::addWaitSemaphore(vk::Semaphore semaphore)
{
    semaphores.push_back(semaphore);
    return *this;
}

std::vector<vk::Result> LavaVk::Core::QueuePresentRequest::present()
{
    std::vector<vk::Result> result(swapchains.size());
    vk::PresentInfoKHR presentInfo{};
    presentInfo.swapchainCount = swapchains.size();
    presentInfo.pSwapchains = swapchains.data();
    presentInfo.pImageIndices = imageIndices.data();
    presentInfo.waitSemaphoreCount = semaphores.size();
    presentInfo.pWaitSemaphores = semaphores.data();
    presentInfo.pResults = result.data();
    queue.presentKHR(&presentInfo);
    return result;
}
