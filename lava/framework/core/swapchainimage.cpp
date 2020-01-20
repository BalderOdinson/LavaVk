//
// Created by dorian on 10. 12. 2019..
//

#include "swapchainimage.h"

#include <utility>
#include "lava/framework/platform/application.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/constants.h"

vk::Image LavaVk::Core::SwapchainImage::getHandle() const
{
    return handle;
}

LavaVk::Core::SharedDevice LavaVk::Core::SwapchainImage::getDevice() const
{
    return device;
}

vk::ImageType LavaVk::Core::SwapchainImage::getImageType() const
{
    return vk::ImageType::e2D;
}

const vk::Extent3D &LavaVk::Core::SwapchainImage::getExtent() const
{
    return extent;
}

vk::Format LavaVk::Core::SwapchainImage::getFormat() const
{
    return format;
}

vk::SampleCountFlagBits LavaVk::Core::SwapchainImage::getSampleCount() const
{
    return vk::SampleCountFlagBits::e1;
}

vk::ImageUsageFlags LavaVk::Core::SwapchainImage::getUsage() const
{
    return imageUsage;
}

vk::ImageTiling LavaVk::Core::SwapchainImage::getTiling() const
{
    return vk::ImageTiling::eOptimal;
}

vk::ImageSubresource LavaVk::Core::SwapchainImage::getSubresource() const
{
    return vk::ImageSubresource({}, 1, 1);
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::SwapchainImage::rebonded()
{
    return rebondedEventSubject;
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::SwapchainImage::recreated()
{
    return recreatedChangedEventSubject;
}

std::type_index LavaVk::Core::SwapchainImage::getType() const
{
    return typeid(SwapchainImage);
}

LavaVk::Core::SharedImageUpdater LavaVk::Core::SwapchainImage::getUpdater()
{
    return Application::instance->container.resolve<ImageUpdater>(Constants::MemoryUpdater::DeviceId);
}

LavaVk::Core::SwapchainImage::SwapchainImage(LavaVk::Core::SharedDevice device, vk::Image handle, vk::Extent3D extent,
                                             vk::Format format, const vk::ImageUsageFlags &imageUsage) : device(
        std::move(device)), handle(handle), extent(extent), format(format), imageUsage(imageUsage)
{
}

void LavaVk::Core::SwapchainImage::recreate(vk::Image h, vk::Extent3D e, vk::Format f, const vk::ImageUsageFlags &iu)
{
    handle = h;
    extent = e;
    imageUsage = iu;
    format = f;

    recreatedChangedEventSubject.raise(*this);
}

LavaVk::Core::SharedImageView LavaVk::Core::SwapchainImage::getView()
{
    return make<ImageView>(
            ImageViewOptions(std::dynamic_pointer_cast<ImageObject>(shared_from_this()), vk::ImageViewType::e2D));
}
