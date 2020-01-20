//
// Created by dorian on 13. 12. 2019..
//

#include "rendercontext.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/rendering/options/rendertargetoptions.h"
#include "lava/framework/rendering/options/renderframeoptions.h"
#include "lava/framework/core/options/queueoptions.h"
#include "lava/framework/resourcecache.h"

LavaVk::RenderContext::RenderContext(LavaVk::InjectionContext &context)
{
    window = context.container.resolve<Window>();
    device = context.container.resolve<Core::Device>();
    queue = context.container.resolve<Core::Queue>();
    allocator = context.container.resolve<Core::Allocator>();
    logger = context.container.resolve<Logger>();
    resourceCache = context.container.resolve<ResourceCache>();
}


LavaVk::RenderContext::~RenderContext()
{
    if (swapchainRecreatedToken)
        swapchain->recreated().unsubscribe(swapchainRecreatedToken);
}

void LavaVk::RenderContext::prepare(size_t threads)
{
    device->waitIdle();

    swapchain = Application::instance->container.resolve<Core::Swapchain>();
    surfaceExtent = swapchain->getExtent();

    for (auto &swapchainImage : swapchain->getImages())
    {
        Application::instance->container.option<RenderTargetOptions>()->recreate(swapchainImage);
        frames.emplace_back(Application::instance->container.resolve<RenderFrame, RenderFrameOptions>(
                Application::instance->container.resolve<RenderTarget>(), threads));
    }

    // Subscribe on surface change.
    swapchain->bindWindow(window);
    swapchainRecreatedToken = swapchain->recreated().subscribe([&](Object &sender)
                                                               { recreate(); });
    threadCount = threads;
    prepared = true;
}

void LavaVk::RenderContext::recreate()
{
    logger->information("Recreated swapchain");
    device->waitIdle();

    auto frameIt = frames.begin();

    for (auto &swapchainImage : swapchain->getImages())
    {
        Application::instance->container.option<RenderTargetOptions>()->recreate(swapchainImage);

        if (frameIt != frames.end())
            (*frameIt)->updateRenderTarget(Application::instance->container.resolve<RenderTarget>());
        else
            // Create a new frame if the new swapchain has more images than current frames
            frames.emplace_back(Application::instance->container.resolve<RenderFrame, RenderFrameOptions>(
                    Application::instance->container.resolve<RenderTarget>(), threadCount));

        ++frameIt;
    }
}

LavaVk::Core::SharedCommandBuffer LavaVk::RenderContext::begin(LavaVk::Core::CommandBuffer::ResetMode resetMode)
{
    assert(prepared && "RenderContext not prepared for rendering, call prepare()");

    acquiredSemaphore = beginFrame();

    if (!acquiredSemaphore)
        throw std::runtime_error("Couldn't begin frame");

    auto[family, properties] = device->getQueueFamilyByUsage(Core::QueueUsage::Graphics).front();
    auto q = Application::instance->container.resolve<Core::Queue, Core::QueueOptions>(Core::QueueIndex(family, 0),
                                                                                       properties);
    return getActiveFrame()->requestCommandBuffer(q, resetMode);
}

void LavaVk::RenderContext::submit(LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    assert(frameActive && "RenderContext is inactive, cannot submit command buffer. Please call begin()");

    Core::SharedSemaphore renderSemaphore{};


    renderSemaphore = submit(queue, commandBuffer, acquiredSemaphore,
                             vk::PipelineStageFlagBits::eColorAttachmentOutput);

    endFrame(renderSemaphore);

    acquiredSemaphore.reset();
}

LavaVk::Core::SharedSemaphore LavaVk::RenderContext::beginFrame()
{
    handleSurfaceChanges();

    assert(!frameActive && "Frame is still active, please call end_frame");

    auto &prevFrame = frames.at(activeFrameIndex);

    auto acquired = prevFrame->requestSemaphore();

    if (swapchain)
    {
        auto fence = prevFrame->requestFence();

        auto result = swapchain->acquireNextImage(activeFrameIndex, acquired, fence);

        if (result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR)
        {
            handleSurfaceChanges();
            result = swapchain->acquireNextImage(activeFrameIndex, acquired, fence);
        }

        if (result != vk::Result::eSuccess)
        {
            prevFrame.reset();
            return nullptr;
        }
    }

    // Now the frame is active again
    frameActive = true;
    allocator->getHandle().setCurrentFrameIndex(activeFrameIndex);
    resourceCache->step();

    waitFrame();

    return acquired;
}

LavaVk::Core::SharedSemaphore LavaVk::RenderContext::submit(const LavaVk::Core::SharedQueue &q,
                                                            const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                            const LavaVk::Core::SharedSemaphore &waitSemaphore,
                                                            const vk::PipelineStageFlags &waitPipelineStage)
{
    auto frame = getActiveFrame();
    auto signalSemaphore = frame->requestSemaphore();
    auto fence = frame->requestFence();

    q->createSubmitRequest()
            .setFence(fence->getHandle())
            .submitInfo()
            .addCommandBuffer(commandBuffer->getHandle())
            .addWaitSemaphore(waitSemaphore->getHandle(), waitPipelineStage)
            .addSignalSemaphore(signalSemaphore->getHandle())
            .add().submit();

    return signalSemaphore;
}

void LavaVk::RenderContext::submit(const LavaVk::Core::SharedQueue &q,
                                   const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    auto frame = getActiveFrame();
    auto fence = frame->requestFence();

    q->createSubmitRequest()
            .setFence(fence->getHandle())
            .submitInfo()
            .addCommandBuffer(commandBuffer->getHandle())
            .add().submit();
}

void LavaVk::RenderContext::waitFrame()
{
    getActiveFrame()->reset();
}

void LavaVk::RenderContext::endFrame(LavaVk::Core::SharedSemaphore semaphore)
{
    assert(frameActive && "Frame is not active, please call begin_frame");

    auto result = queue->createPresentRequest()
            .addWaitSemaphore(semaphore->getHandle())
            .addSwapchain(swapchain->getHandle(), activeFrameIndex)
            .present();

    if (result.front() == vk::Result::eSuboptimalKHR || result.front() == vk::Result::eErrorOutOfDateKHR)
        handleSurfaceChanges();

    // Frame is not active anymore
    frameActive = false;
}

const LavaVk::SharedRenderFrame &LavaVk::RenderContext::getActiveFrame() const
{
    assert(frameActive && "Frame is not active, please call beginFrame");
    return frames.at(activeFrameIndex);
}

uint32_t LavaVk::RenderContext::getActiveFrameIndex() const
{
    assert(frameActive && "Frame is not active, please call beginFrame");
    return activeFrameIndex;
}

const LavaVk::SharedRenderFrame &LavaVk::RenderContext::getLastRenderedFrame() const
{
    assert(!frameActive && "Frame is still active, please call endFrame");
    return frames.at(activeFrameIndex);
}

LavaVk::Core::SharedSemaphore LavaVk::RenderContext::requestSemaphore()
{
    return getActiveFrame()->requestSemaphore();
}

std::type_index LavaVk::RenderContext::getType() const
{
    return typeid(RenderContext);
}

void LavaVk::RenderContext::handleSurfaceChanges()
{
    auto surfaceProperties = device->getPhysicalDevice().getSurfaceCapabilitiesKHR(
            Application::instance->container.resolve<Core::Surface>()->getHandle());

    // Only recreate the swapchain if the dimensions have changed;
    // handle_surface_changes() is called on VK_SUBOPTIMAL_KHR,
    // which might not be due to a surface resize
    if (surfaceProperties.currentExtent.width != surfaceExtent.width ||
        surfaceProperties.currentExtent.height != surfaceExtent.height)
    {
        // Recreate swapchain
        device->waitIdle();

        auto options = Application::instance->container.option<Core::SwapchainOptions>();
        options->extent = surfaceProperties.currentExtent;
        options->transform = preTransform;

        swapchain->recreate(options);

        surfaceExtent = surfaceProperties.currentExtent;
    }
}

const LavaVk::SharedRenderFrame &LavaVk::RenderContext::getRenderFrame(uint32_t index) const
{
    return frames[index];
}
