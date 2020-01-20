//
// Created by dorian on 10. 12. 2019..
//

#include "imageview.h"
#include "lava/framework/vkcommon.h"
#include "lava/framework/platform/application.h"

#include <utility>

LavaVk::Core::ImageView::ImageView(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    options = *context.container.option<ImageViewOptions>(context);

    if (options.format == vk::Format::eUndefined)
        options.format = options.image->getFormat();

    subresourceRange.levelCount = options.image->getSubresource().mipLevel;
    subresourceRange.layerCount = options.image->getSubresource().arrayLayer;

    if (isDepthOnlyFormat(options.format))
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
    else if (isDepthStencilFormat(options.format))
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
    else
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    vk::ImageViewCreateInfo info{};
    info.image = options.image->getHandle();
    info.viewType = options.viewType;
    info.format = options.format;
    info.subresourceRange = subresourceRange;

    handle = device->getHandle().createImageView(info);
    rebindToken = options.image->rebonded().subscribe(
            std::bind(&LavaVk::Core::ImageView::recreate, this, std::placeholders::_1));
    recreateToken = options.image->recreated().subscribe(
            std::bind(&LavaVk::Core::ImageView::recreate, this, std::placeholders::_1));
}

LavaVk::Core::ImageView::ImageView(LavaVk::Core::ImageViewOptions o)
        : options(std::move(o))
{
    device = options.image->getDevice();

    if (options.format == vk::Format::eUndefined)
        options.format = options.image->getFormat();

    subresourceRange.levelCount = options.image->getSubresource().mipLevel;
    subresourceRange.layerCount = options.image->getSubresource().arrayLayer;

    if (isDepthOnlyFormat(options.format))
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
    else if (isDepthStencilFormat(options.format))
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
    else
        subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    vk::ImageViewCreateInfo info{};
    info.image = options.image->getHandle();
    info.viewType = options.viewType;
    info.format = options.format;
    info.subresourceRange = subresourceRange;

    handle = device->getHandle().createImageView(info);
    rebindToken = options.image->rebonded().subscribe(
            std::bind(&LavaVk::Core::ImageView::recreate, this, std::placeholders::_1));
    recreateToken = options.image->recreated().subscribe(
            std::bind(&LavaVk::Core::ImageView::recreate, this, std::placeholders::_1));
}

LavaVk::Core::ImageView::~ImageView()
{
    if (recreateToken)
        options.image->recreated().unsubscribe(recreateToken);
    if (rebindToken)
        options.image->rebonded().unsubscribe(rebindToken);

    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::ImageView LavaVk::Core::ImageView::getHandle() const
{
    return handle;
}

LavaVk::Core::SharedImageObject LavaVk::Core::ImageView::getImage() const
{
    return options.image;
}

vk::Format LavaVk::Core::ImageView::getFormat() const
{
    return options.format;
}

vk::ImageViewType LavaVk::Core::ImageView::getImageViewType() const
{
    return options.viewType;
}

vk::ImageSubresourceRange LavaVk::Core::ImageView::getSubresourceRange() const
{
    return subresourceRange;
}

vk::ImageSubresourceLayers LavaVk::Core::ImageView::getSubresourceLayers() const
{
    vk::ImageSubresourceLayers subresource{};
    subresource.aspectMask = subresourceRange.aspectMask;
    subresource.baseArrayLayer = subresourceRange.baseArrayLayer;
    subresource.layerCount = subresourceRange.layerCount;
    subresource.mipLevel = subresourceRange.baseMipLevel;
    return subresource;
}

std::type_index LavaVk::Core::ImageView::getType() const
{
    return typeid(ImageView);
}

void LavaVk::Core::ImageView::recreate(LavaVk::Object &newImage)
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }

    vk::ImageViewCreateInfo info{};
    info.image = options.image->getHandle();
    info.viewType = options.viewType;
    info.format = options.format;
    info.subresourceRange = subresourceRange;

    handle = device->getHandle().createImageView(info);
}
