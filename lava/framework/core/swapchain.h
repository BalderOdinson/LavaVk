//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_SWAPCHAIN_H
#define LAVAVK_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "options/swapchainoptions.h"
#include "device.h"
#include "swapchainimage.h"
#include "semaphore.h"
#include "fence.h"
#include "lava/framework/platform/logger.h"
#include "lava/framework/platform/window.h"

namespace LavaVk::Core
{
    class Swapchain : public virtual Object
    {
    public:
        explicit Swapchain(InjectionContext &context);
        Swapchain(const Swapchain &) = delete;
        Swapchain(Swapchain &&) = delete;
        ~Swapchain() override;

        Swapchain &operator=(const Swapchain &) = delete;
        Swapchain &operator=(Swapchain &&) = delete;

        vk::SwapchainKHR getHandle() const;
        vk::Extent2D getExtent() const;
        vk::Format getFormat() const;
        const std::vector<SharedSwapchainImage> &getImages() const;
        vk::SurfaceTransformFlagBitsKHR getTransform() const;
        vk::ImageUsageFlags getUsage() const;
        vk::PresentModeKHR getPresentMode() const;

        void bindWindow(SharedWindow window);

        Event<EventHandler<>> &recreated();

        void recreate(const SharedSwapchainOptions& options);

        vk::Result
        acquireNextImage(uint32_t &imageIndex, const SharedSemaphore &imageAcquiredSemaphore, const SharedFence &fence);

        std::type_index getType() const override;

    private:
        EventOwner<EventHandler<>> recreatedChangedEventSubject;
        SharedDevice device;
        SharedSurface surface;
        SharedLogger logger;
        SharedWindow window;
        vk::SwapchainKHR handle;
        SwapchainOptions options;
        vk::Format format;
        uint32_t imageAvailable;
        std::vector<SharedSwapchainImage> images;
        EventToken windowChangedToken;
    };

    using UniqueSwapchain = std::unique_ptr<Swapchain>;
    using SharedSwapchain = std::shared_ptr<Swapchain>;
    using WeakSwapchain = std::weak_ptr<Swapchain>;
}


#endif //LAVAVK_SWAPCHAIN_H
