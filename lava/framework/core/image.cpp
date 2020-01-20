//
// Created by dorian on 10. 12. 2019..
//

#include "image.h"
#include "buffer.h"
#include "image2buffertransfer.h"
#include "options/stagingbufferoptions.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/vkcommon.h"
#include "lava/framework/constants.h"
#include "lava/framework/platform/application.h"

#include <utility>

LavaVk::Core::Image::Image(LavaVk::InjectionContext &context)
{
    allocator = context.container.resolve<Allocator>();
    options = *context.container.option<ImageOptions>(context);
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.usage = options.memoryUsage;
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;

    vk::ImageCreateInfo info{};
    info.imageType = findImageType(options.extent);
    info.format = options.format;
    info.extent = options.extent;
    info.mipLevels = options.mipLevels;
    info.arrayLayers = options.arrayLayers;
    info.samples = options.sampleCount;
    info.tiling = options.imageTiling;
    info.usage = options.imageUsage;
    info.flags = options.flags;
    info.initialLayout = vk::ImageLayout::eUndefined;

    auto[image, allocation] = allocator->getHandle().createImage(info, allocationCreateInfo);
    handle = image;
    allocationHandle = allocation;
}

LavaVk::Core::Image::Image(LavaVk::Core::ImageOptions o)
        : allocator(Application::instance->container.resolve<Allocator>()), options(std::move(o))
{
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.usage = options.memoryUsage;
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;

    vk::ImageCreateInfo info{};
    info.imageType = findImageType(options.extent);
    info.format = options.format;
    info.extent = options.extent;
    info.mipLevels = options.mipLevels;
    info.arrayLayers = options.arrayLayers;
    info.samples = options.sampleCount;
    info.tiling = options.imageTiling;
    info.usage = options.imageUsage;
    info.flags = options.flags;
    info.initialLayout = vk::ImageLayout::eUndefined;

    auto[image, allocation] = allocator->getHandle().createImage(info, allocationCreateInfo);
    handle = image;
    allocationHandle = allocation;
}

LavaVk::Core::Image::~Image()
{
    if (handle)
    {
        allocator->getHandle().destroyImage(handle, allocationHandle);
        handle = nullptr;
        allocationHandle = nullptr;
    }
}

vk::Image LavaVk::Core::Image::getHandle() const
{
    return handle;
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::Image::rebonded()
{
    return rebondedEventSubject;
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::Image::recreated()
{
    return recreatedChangedEventSubject;
}

vma::Allocation LavaVk::Core::Image::getAllocation() const
{
    return allocationHandle;
}

LavaVk::Core::SharedAllocator LavaVk::Core::Image::getAllocator() const
{
    return allocator;
}

void LavaVk::Core::Image::recreate()
{
    allocator->getHandle().destroyImage(handle, allocationHandle);
    vma::AllocationCreateInfo allocationCreateInfo{};
    if (options.isMapped)
        allocationCreateInfo.flags = vma::AllocationCreateFlagBits::eMapped;
    if (options.canGetLost)
    {
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanBecomeLost;
        allocationCreateInfo.flags |= vma::AllocationCreateFlagBits::eCanMakeOtherLost;
    }
    allocationCreateInfo.usage = options.memoryUsage;
    allocationCreateInfo.preferredFlags = options.preferredFlags;
    allocationCreateInfo.requiredFlags = options.requiredFlags;

    vk::ImageCreateInfo info{};
    info.imageType = findImageType(options.extent);
    info.format = options.format;
    info.extent = options.extent;
    info.mipLevels = options.mipLevels;
    info.arrayLayers = options.arrayLayers;
    info.samples = options.sampleCount;
    info.tiling = options.imageTiling;
    info.usage = options.imageUsage;
    info.flags = options.flags;
    info.initialLayout = vk::ImageLayout::eUndefined;

    auto[image, allocation] = allocator->getHandle().createImage(info, allocationCreateInfo);
    handle = image;
    allocationHandle = allocation;
    recreatedChangedEventSubject.raise(*this);
}

void LavaVk::Core::Image::rebind()
{
    allocator->getDevice()->getHandle().destroyImage(handle);
    vk::ImageCreateInfo info{};
    info.imageType = findImageType(options.extent);
    info.format = options.format;
    info.extent = options.extent;
    info.mipLevels = options.mipLevels;
    info.arrayLayers = options.arrayLayers;
    info.samples = options.sampleCount;
    info.tiling = options.imageTiling;
    info.usage = options.imageUsage;
    info.flags = options.flags;
    info.initialLayout = vk::ImageLayout::eUndefined;
    handle = allocator->getDevice()->getHandle().createImage(info);
    rebondedEventSubject.raise(*this);
}

std::type_index LavaVk::Core::Image::getType() const
{
    return typeid(Image);
}

vk::ImageType LavaVk::Core::Image::findImageType(vk::Extent3D extent)
{
    vk::ImageType result{};

    uint32_t dimNum{0};

    if (extent.width >= 1) ++dimNum;
    if (extent.height >= 1) ++dimNum;
    if (extent.depth > 1) ++dimNum;

    switch (dimNum)
    {
        case 1:
            result = vk::ImageType::e1D;
            break;
        case 2:
            result = vk::ImageType::e2D;
            break;
        case 3:
            result = vk::ImageType::e3D;
            break;
        default:
            throw std::runtime_error("No image type found.");
            break;
    }

    return result;
}

vk::ImageType LavaVk::Core::Image::getImageType() const
{
    return findImageType(options.extent);
}

const vk::Extent3D &LavaVk::Core::Image::getExtent() const
{
    return options.extent;
}

vk::Format LavaVk::Core::Image::getFormat() const
{
    return options.format;
}

vk::SampleCountFlagBits LavaVk::Core::Image::getSampleCount() const
{
    return options.sampleCount;
}

vk::ImageUsageFlags LavaVk::Core::Image::getUsage() const
{
    return options.imageUsage;
}

vk::ImageTiling LavaVk::Core::Image::getTiling() const
{
    return options.imageTiling;
}

vk::ImageSubresource LavaVk::Core::Image::getSubresource() const
{
    return vk::ImageSubresource({}, options.mipLevels, options.arrayLayers);
}

LavaVk::Core::SharedDevice LavaVk::Core::Image::getDevice() const
{
    return allocator->getDevice();
}

void LavaVk::Core::Image::getData(void *data, size_t *size) const
{
    *size = options.extent.height * options.extent.width * options.extent.depth * getBitsPerPixel(options.format);

    if (!data)
        return;

    auto allocationInfo = getAllocationInfo();
    auto memFlags = allocator->getHandle().getMemoryTypeProperties(allocationInfo.memoryType);
    if (memFlags & vk::MemoryPropertyFlagBits::eHostVisible)
    {
        if (allocationInfo.pMappedData)
        {
            invalidate(0, VK_WHOLE_SIZE);
            memcpy(data, allocationInfo.pMappedData, *size);
        }
        else
        {
            memcpy(data, map(), *size);
            unmap();
        }
    }
    else
        throw std::runtime_error("Buffer is located on GPU. To get data issue a Buffer copy command!");
}

LavaVk::Core::SharedImageUpdater LavaVk::Core::Image::getUpdater()
{
    return Application::instance->container.resolve<ImageUpdater>(Constants::MemoryUpdater::DeviceId);
}
