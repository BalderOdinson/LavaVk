//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_SWAPCHAINIMAGE_H
#define LAVAVK_SWAPCHAINIMAGE_H

#include "imageobject.h"
#include "device.h"
#include "imageview.h"

namespace LavaVk::Core
{
    class SwapchainImage : public ImageObject
    {
    public:
        SwapchainImage(SharedDevice device, vk::Image handle, vk::Extent3D extent, vk::Format format, const vk::ImageUsageFlags& imageUsage);

        vk::Image getHandle() const override;
        SharedDevice getDevice() const override;
        vk::ImageType getImageType() const override;
        const vk::Extent3D &getExtent() const override;
        vk::Format getFormat() const override;
        vk::SampleCountFlagBits getSampleCount() const override;
        vk::ImageUsageFlags getUsage() const override;
        vk::ImageTiling getTiling() const override;
        vk::ImageSubresource getSubresource() const override;

        Event<EventHandler<>> &rebonded() override;
        Event<EventHandler<>> &recreated() override;
        std::type_index getType() const override;

        void recreate(vk::Image handle, vk::Extent3D extent, vk::Format format, const vk::ImageUsageFlags& imageUsage);
        SharedImageView getView();

    protected:
        SharedImageUpdater getUpdater() override;

    private:
        SharedDevice device;
        vk::Image handle;
        vk::Extent3D extent;
        vk::Format format;
        vk::ImageUsageFlags imageUsage;
        EventOwner<EventHandler<>> rebondedEventSubject;
        EventOwner<EventHandler<>> recreatedChangedEventSubject;
    };

    using UniqueSwapchainImage = std::unique_ptr<SwapchainImage>;
    using SharedSwapchainImage = std::shared_ptr<SwapchainImage>;
    using WeakSwapchainImage = std::weak_ptr<SwapchainImage>;
}


#endif //LAVAVK_SWAPCHAINIMAGE_H
