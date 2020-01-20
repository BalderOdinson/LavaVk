//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_QUEUEPRESENTREQUEST_H
#define LAVAVK_QUEUEPRESENTREQUEST_H

#include <vulkan/vulkan.hpp>
#include <vector>

namespace LavaVk::Core
{
    class QueuePresentRequest
    {
    public:
        explicit QueuePresentRequest(vk::Queue queue);

        QueuePresentRequest &addSwapchain(vk::SwapchainKHR swapchain, uint32_t imageIndex);
        QueuePresentRequest &addWaitSemaphore(vk::Semaphore semaphore);

        std::vector<vk::Result> present();

    private:
        vk::Queue queue;
        std::vector<vk::SwapchainKHR> swapchains;
        std::vector<uint32_t> imageIndices;
        std::vector<vk::Semaphore> semaphores;
    };
}


#endif //LAVAVK_QUEUEPRESENTREQUEST_H
