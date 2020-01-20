//
// Created by dorian on 10. 12. 2019..
//

#include "swapchain.h"
#include <set>
#include <utility>
#include "lava/framework/platform/application.h"

inline uint32_t chooseImageCount(
        uint32_t requestImageCount,
        uint32_t minImageCount,
        uint32_t maxImageCount)
{
    requestImageCount = std::min(requestImageCount, maxImageCount);
    requestImageCount = std::max(requestImageCount, minImageCount);

    return requestImageCount;
}

inline uint32_t chooseImageArrayLayers(
        uint32_t requestImageArrayLayers,
        uint32_t maxImageArrayLayers)
{
    requestImageArrayLayers = std::min(requestImageArrayLayers, maxImageArrayLayers);
    requestImageArrayLayers = std::max(requestImageArrayLayers, 1u);

    return requestImageArrayLayers;
}

inline vk::Extent2D chooseExtent(
        vk::Extent2D requestExtent,
        const vk::Extent2D &minImageExtent,
        const vk::Extent2D &maxImageExtent,
        const vk::Extent2D &currentExtent,
        const LavaVk::SharedLogger &logger)
{
    if (requestExtent.width < 1 || requestExtent.height < 1)
        return currentExtent;

    requestExtent.width = std::max(requestExtent.width, minImageExtent.width);
    requestExtent.width = std::min(requestExtent.width, maxImageExtent.width);

    requestExtent.height = std::max(requestExtent.height, minImageExtent.height);
    requestExtent.height = std::min(requestExtent.height, maxImageExtent.height);

    return requestExtent;
}

inline vk::PresentModeKHR choosePresentMode(
        vk::PresentModeKHR requestPresentMode,
        const std::vector<vk::PresentModeKHR> &presentModes,
        const LavaVk::SharedLogger &logger)
{
    auto presentModeIt = std::find(presentModes.begin(), presentModes.end(), requestPresentMode);

    if (presentModeIt == presentModes.end())
    {
        presentModeIt = presentModes.begin();
        logger->warning("Requested present mode not supported. Selected default value.");
    }

    return *presentModeIt;
}

inline vk::SurfaceFormatKHR chooseSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR> &requestSurfaceFormats,
        const std::vector<vk::SurfaceFormatKHR> &surfaceFormats,
        const LavaVk::SharedLogger &logger)
{
    std::vector<vk::SurfaceFormatKHR>::const_iterator surfaceFormatIt;

    for (auto &requestSurfaceFormat : requestSurfaceFormats)
    {
        surfaceFormatIt = std::find_if(
                surfaceFormats.begin(),
                surfaceFormats.end(),
                [&requestSurfaceFormat](const vk::SurfaceFormatKHR &surface)
                {
                    return surface.format == requestSurfaceFormat.format &&
                           surface.colorSpace == requestSurfaceFormat.colorSpace;
                });

        if (surfaceFormatIt != surfaceFormats.end())
            break;
    }

    if (surfaceFormatIt == surfaceFormats.end())
    {
        surfaceFormatIt = surfaceFormats.begin();
        logger->warning("Requested surface format not supported. Selected default value.");
    }

    return *surfaceFormatIt;
}

inline vk::SurfaceTransformFlagBitsKHR chooseTransform(
        vk::SurfaceTransformFlagBitsKHR requestTransform,
        const vk::SurfaceTransformFlagsKHR &supportedTransform,
        vk::SurfaceTransformFlagBitsKHR currentTransform,
        const LavaVk::SharedLogger &logger)
{
    if (requestTransform & supportedTransform)
        return requestTransform;

    logger->warning("Requested transform not supported. Selected default value.");

    return currentTransform;
}

inline vk::CompositeAlphaFlagBitsKHR chooseCompositeAlpha(vk::CompositeAlphaFlagBitsKHR requestCompositeAlpha,
                                                          const vk::CompositeAlphaFlagsKHR &supportedCompositeAlpha,
                                                          const LavaVk::SharedLogger &logger)
{
    if (requestCompositeAlpha & supportedCompositeAlpha)
        return requestCompositeAlpha;

    logger->warning("Requested composite alpha not supported. Selected default value.");

    static const std::vector<vk::CompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
            vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
            vk::CompositeAlphaFlagBitsKHR::eInherit};

    for (vk::CompositeAlphaFlagBitsKHR composite_alpha : compositeAlphaFlags)
        if (composite_alpha & supportedCompositeAlpha)
            return composite_alpha;

    throw std::runtime_error("No compatible composite alpha found.");
}

inline vk::ImageUsageFlags
chooseImageUsage(const vk::ImageUsageFlags &requestedImageUsage, const vk::ImageUsageFlags &supportedImageUsage)
{
    if (requestedImageUsage & supportedImageUsage)
        return requestedImageUsage;

    static const std::vector<vk::ImageUsageFlagBits> imageUsageFlags = {
            vk::ImageUsageFlagBits::eColorAttachment,
            vk::ImageUsageFlagBits::eStorage,
            vk::ImageUsageFlagBits::eSampled,
            vk::ImageUsageFlagBits::eTransferDst};

    for (vk::ImageUsageFlagBits imageUsage : imageUsageFlags)
        if (imageUsage & supportedImageUsage)
            return imageUsage;

    throw std::runtime_error("No compatible image usage found.");
}

inline vk::ImageUsageFlags compositeImageFlags(const std::set<vk::ImageUsageFlagBits> &imageUsageFlags)
{
    vk::ImageUsageFlags imageUsage{};
    for (auto flag : imageUsageFlags)
        imageUsage |= flag;

    return imageUsage;
}

LavaVk::Core::Swapchain::Swapchain(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    surface = context.container.resolve<Surface>();
    logger = context.container.resolve<Logger>();
    options = *context.container.option<SwapchainOptions>(context);

    auto surfaceCapabilities = device->getPhysicalDevice().getSurfaceCapabilitiesKHR(surface->getHandle());
    auto surfaceFormats = device->getPhysicalDevice().getSurfaceFormatsKHR(surface->getHandle());
    auto presentModes = device->getPhysicalDevice().getSurfacePresentModesKHR(surface->getHandle());
    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.minImageCount = chooseImageCount(options.imageCount, surfaceCapabilities.minImageCount,
                                                surfaceCapabilities.maxImageCount);
    createInfo.imageExtent = chooseExtent(options.extent, surfaceCapabilities.minImageExtent,
                                          surfaceCapabilities.maxImageExtent, surfaceCapabilities.currentExtent,
                                          logger);
    std::vector<vk::SurfaceFormatKHR> requestSurfaceFormats{
            vk::SurfaceFormatKHR({VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}),
            vk::SurfaceFormatKHR({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}),
    };
    auto surfaceFormat = chooseSurfaceFormat(requestSurfaceFormats, surfaceFormats, logger);
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;

    createInfo.imageArrayLayers = chooseImageArrayLayers(1u, surfaceCapabilities.maxImageArrayLayers);
    createInfo.imageUsage = chooseImageUsage(options.imageUsage, surfaceCapabilities.supportedUsageFlags);

    createInfo.preTransform = chooseTransform(options.transform, surfaceCapabilities.supportedTransforms,
                                              surfaceCapabilities.currentTransform, logger);
    createInfo.compositeAlpha = chooseCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                     surfaceCapabilities.supportedCompositeAlpha, logger);
    createInfo.presentMode = choosePresentMode(options.presentMode, presentModes, logger);

    createInfo.surface = surface->getHandle();

    handle = device->getHandle().createSwapchainKHR(createInfo);

    options.extent = createInfo.imageExtent;
    options.imageCount = createInfo.minImageCount;
    options.transform = createInfo.preTransform;
    options.presentMode = createInfo.presentMode;
    format = createInfo.imageFormat;
    options.imageUsage = createInfo.imageUsage;

    auto swapchainImages = device->getHandle().getSwapchainImagesKHR(handle);
    imageAvailable = swapchainImages.size();
    images.resize(imageAvailable);
    for (uint32_t i = 0; i < imageAvailable; ++i)
        images[i] = make<SwapchainImage>(device, swapchainImages[i], vk::Extent3D(options.extent, 1), format,
                                         options.imageUsage);
}

LavaVk::Core::Swapchain::~Swapchain()
{
    if (windowChangedToken)
        window->resized().unsubscribe(windowChangedToken);

    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::SwapchainKHR LavaVk::Core::Swapchain::getHandle() const
{
    return handle;
}

vk::Extent2D LavaVk::Core::Swapchain::getExtent() const
{
    return options.extent;
}

vk::Format LavaVk::Core::Swapchain::getFormat() const
{
    return format;
}

const std::vector<LavaVk::Core::SharedSwapchainImage> &LavaVk::Core::Swapchain::getImages() const
{
    return images;
}

vk::SurfaceTransformFlagBitsKHR LavaVk::Core::Swapchain::getTransform() const
{
    return options.transform;
}

vk::ImageUsageFlags LavaVk::Core::Swapchain::getUsage() const
{
    return options.imageUsage;
}

vk::PresentModeKHR LavaVk::Core::Swapchain::getPresentMode() const
{
    return options.presentMode;
}

vk::Result LavaVk::Core::Swapchain::acquireNextImage(uint32_t &imageIndex,
                                                     const LavaVk::Core::SharedSemaphore &imageAcquiredSemaphore,
                                                     const LavaVk::Core::SharedFence &fence)
{
    return device->getHandle().acquireNextImageKHR(handle, std::numeric_limits<uint64_t>::max(),
                                                   imageAcquiredSemaphore->getHandle(), fence->getHandle(),
                                                   &imageIndex);
}

void LavaVk::Core::Swapchain::recreate(const LavaVk::Core::SharedSwapchainOptions& o)
{
    options = *o;
    auto surfaceCapabilities = device->getPhysicalDevice().getSurfaceCapabilitiesKHR(surface->getHandle());
    auto surfaceFormats = device->getPhysicalDevice().getSurfaceFormatsKHR(surface->getHandle());
    auto presentModes = device->getPhysicalDevice().getSurfacePresentModesKHR(surface->getHandle());
    vk::SwapchainCreateInfoKHR createInfo{};
    createInfo.minImageCount = chooseImageCount(options.imageCount, surfaceCapabilities.minImageCount,
                                                surfaceCapabilities.maxImageCount);
    createInfo.imageExtent = chooseExtent(options.extent, surfaceCapabilities.minImageExtent,
                                          surfaceCapabilities.maxImageExtent, surfaceCapabilities.currentExtent,
                                          logger);
    std::vector<vk::SurfaceFormatKHR> requestSurfaceFormats{
            vk::SurfaceFormatKHR({VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}),
            vk::SurfaceFormatKHR({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}),
    };
    auto surfaceFormat = chooseSurfaceFormat(requestSurfaceFormats, surfaceFormats, logger);
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;

    createInfo.imageArrayLayers = chooseImageArrayLayers(1u, surfaceCapabilities.maxImageArrayLayers);
    createInfo.imageUsage = chooseImageUsage(options.imageUsage, surfaceCapabilities.supportedUsageFlags);

    createInfo.preTransform = chooseTransform(options.transform, surfaceCapabilities.supportedTransforms,
                                              surfaceCapabilities.currentTransform, logger);
    createInfo.compositeAlpha = chooseCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                     surfaceCapabilities.supportedCompositeAlpha, logger);
    createInfo.presentMode = choosePresentMode(options.presentMode, presentModes, logger);

    createInfo.surface = surface->getHandle();
    createInfo.oldSwapchain = handle;

    handle = device->getHandle().createSwapchainKHR(createInfo);
    device->getHandle().destroy(createInfo.oldSwapchain);

    options.extent = createInfo.imageExtent;
    options.imageCount = createInfo.minImageCount;
    options.transform = createInfo.preTransform;
    options.presentMode = createInfo.presentMode;
    format = createInfo.imageFormat;
    options.imageUsage = createInfo.imageUsage;

    auto swapchainImages = device->getHandle().getSwapchainImagesKHR(handle);
    imageAvailable = swapchainImages.size();
    images.resize(imageAvailable);
    for (uint32_t i = 0; i < imageAvailable; ++i)
        images[i] = make<SwapchainImage>(device, swapchainImages[i], vk::Extent3D(options.extent, 1), format,
                                         options.imageUsage);

    recreatedChangedEventSubject.raise(*this);
}

LavaVk::Event<LavaVk::EventHandler<>> &LavaVk::Core::Swapchain::recreated()
{
    return recreatedChangedEventSubject;
}

std::type_index LavaVk::Core::Swapchain::getType() const
{
    return typeid(Swapchain);
}

void LavaVk::Core::Swapchain::bindWindow(LavaVk::SharedWindow newWindow)
{
    if (window && windowChangedToken)
        window->resized().unsubscribe(windowChangedToken);

    window = std::move(newWindow);
    windowChangedToken = window->resized().subscribe(
            [&](Object &sender, const ResizeEventArgs &args)
            {
                options.extent = vk::Extent2D(args.width, args.height);
                recreate(Application::instance->container.option<SwapchainOptions>());
            });
}
