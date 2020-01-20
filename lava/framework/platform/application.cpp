//
// Created by dorian on 07. 12. 2019..
//

#include "application.h"
#include "inputmanager.h"
#include "shared/defaultlogger.h"
#include "lava/core.h"
#include "lava/dependencyinjection.h"
#include "lava/framework.h"
#include "lava/scene-graph.h"
#include "lava/loaders.h"
#include "lava/rendering.h"

LavaVk::UniqueApplication LavaVk::Application::instance = nullptr;

void LavaVk::Application::installDependencies(LavaVk::DIContainer &diContainer)
{
    diContainer.bind<InputManager>().toSelf().asSingleton(new InputManager);
    diContainer.bind<Timer>().toSelf().asSingleton(new Timer());
    diContainer.bind<Time>().toSelf().asSingleton(new Time());
    diContainer.bind<FrameCounter>().toSelf().asLazySingleton();
    diContainer.bind<Logger>().to<DefaultLogger>().asLazySingleton();
    diContainer.bind<Core::Instance>().toSelf().asLazySingleton().withOption<Core::InstanceOptions>(
            [](InjectionContext context)
            {
                auto appOptions = context.container.option<ApplicationOption>();
                appOptions->additionalRequiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
                appOptions->additionalRequiredExtensions.push_back(
                        context.container.resolve<Window>()->getSurfaceExtensions());
                if (appOptions->enableDebug)
                {
                    appOptions->additionalRequiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
                    appOptions->additionalRequiredLayers.push_back("VK_LAYER_KHRONOS_validation");
                    appOptions->additionalRequiredLayers.push_back("VK_LAYER_LUNARG_standard_validation");
                }
                auto option = new Core::InstanceOptions(appOptions->appName, appOptions->version,
                                                        appOptions->additionalRequiredExtensions,
                                                        appOptions->additionalRequiredLayers);

                return option;
            });
    diContainer.bind<Core::Surface>().toSelf().asLazySingleton();
    diContainer.bind<Core::Device>().toSelf().asLazySingleton().withOption<Core::DeviceOptions>(
            [](InjectionContext context)
            {
                auto global = context.container.option<Core::DeviceOptions>();
                auto options = new Core::DeviceOptions(*global);
                options->extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                options->extensions.push_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
                options->extensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
                auto instance = context.container.resolve<Core::Instance>();
                auto features = instance->getGpu().getFeatures();
                if (features.textureCompressionASTC_LDR)
                    options->requestedFeatures.textureCompressionASTC_LDR = true;
                else if (features.textureCompressionBC)
                    options->requestedFeatures.textureCompressionBC = true;
                else if (features.textureCompressionETC2)
                    options->requestedFeatures.textureCompressionETC2 = true;
                if (features.samplerAnisotropy)
                    options->requestedFeatures.samplerAnisotropy = true;
                if (features.geometryShader)
                    options->requestedFeatures.geometryShader = true;
                if (features.wideLines)
                    options->requestedFeatures.wideLines = true;
                if (features.fillModeNonSolid)
                    options->requestedFeatures.fillModeNonSolid = true;

                return options;
            });
    diContainer.bind<Core::Queue>().toSelf().asScoped();
    diContainer.bind<Core::Queue>().toSelf(
            Constants::Queue::MainThreadTransferId).asLazySingleton().withOption<Core::QueueOptions>(
            [](InjectionContext context)
            {
                auto device = context.container.resolve<Core::Device>();
                auto properties = device->getQueueFamilyByUsage(Core::QueueUsage::Transfer);
                return new Core::QueueOptions({properties.front().first, 0},
                                              properties.front().second);
            });
    diContainer.bind<Core::Queue>().toSelf(
            Constants::Queue::WorkerThreadTransferId).asLazySingleton().withOption<Core::QueueOptions>(
            [](InjectionContext context)
            {
                auto device = context.container.resolve<Core::Device>();
                auto properties = device->getQueueFamilyByUsage(Core::QueueUsage::Transfer);
                return new Core::QueueOptions({properties.front().first, properties.front().second.queueCount - 1},
                                              properties.front().second);
            });
    diContainer.bind<Core::Allocator>().toSelf().asLazySingleton();
    diContainer.bind<Core::Fence>().toSelf().asScoped().withOption<Core::FenceOptions>(
            []()
            { return new Core::FenceOptions(Core::FenceState::Unsignaled); });
    diContainer.bind<Core::Semaphore>().toSelf().asScoped();
    diContainer.bind<Core::CommandPool>().toSelf().asScoped();
    diContainer.bind<Core::CommandPool>().toSelf(
            Constants::CommandPool::GlobalPoolId).asLazySingleton().withOption<Core::CommandPoolOptions>(
            [](InjectionContext ctx)
            {
                auto family = ctx.container.resolve<Core::Queue>()->getIndex().family;
                return new Core::CommandPoolOptions(Core::CommandBuffer::ResetMode::ResetIndividually, true, family, 0);
            });
    diContainer.bind<Core::CommandPool>().toSelf(
            Constants::CommandPool::MainThreadTransferPoolId).asLazySingleton().withOption<Core::CommandPoolOptions>(
            [](InjectionContext ctx)
            {
                auto family = ctx.container.resolve<Core::Queue>(
                        Constants::Queue::MainThreadTransferId)->getIndex().family;
                return new Core::CommandPoolOptions(Core::CommandBuffer::ResetMode::ResetIndividually, true, family, 0);
            });
    diContainer.bind<Core::CommandPool>().toSelf(
            Constants::CommandPool::WorkerThreadTransferPoolId).asLazySingleton().withOption<Core::CommandPoolOptions>(
            [](InjectionContext ctx)
            {
                auto family = ctx.container.resolve<Core::Queue>(
                        Constants::Queue::WorkerThreadTransferId)->getIndex().family;
                return new Core::CommandPoolOptions(Core::CommandBuffer::ResetMode::ResetIndividually, true, family, 1);
            });
    diContainer.bind<Core::CommandBuffer>().toSelf(
            Constants::CommandBuffer::MainThreadTransferId).asLazySingleton(
            [](InjectionContext ctx)
            {
                auto commandPool = ctx.container.resolve<Core::CommandPool>(
                        Constants::CommandPool::MainThreadTransferPoolId);
                return commandPool->requestBuffer(vk::CommandBufferLevel::ePrimary);
            });
    diContainer.bind<Core::CommandBuffer>().toSelf(
            Constants::CommandBuffer::WorkerThreadTransferId).asLazySingleton(
            [](InjectionContext ctx)
            {
                auto commandPool = ctx.container.resolve<Core::CommandPool>(
                        Constants::CommandPool::WorkerThreadTransferPoolId);
                return commandPool->requestBuffer(vk::CommandBufferLevel::ePrimary);
            });
    diContainer.bind<Core::BufferUpdater>().to<Core::HostBufferUpdater>(Constants::MemoryUpdater::HostId).asScoped();
    diContainer.bind<Core::BufferUpdater>().to<Core::DeviceBufferUpdater>(
            Constants::MemoryUpdater::DeviceId).asScoped().withOption<Core::DeviceMemoryUpdateOptions>(
            [](InjectionContext context)
            {
                return new Core::DeviceMemoryUpdateOptions(
                        context.container.resolve<Core::Queue>(Constants::Queue::MainThreadTransferId),
                        context.container.resolve<Core::Fence>(),
                        context.container.resolve<Core::CommandPool>(Constants::CommandPool::MainThreadTransferPoolId));
            });
    diContainer.bind<Core::ImageUpdater>().to<Core::DeviceImageUpdater>(
            Constants::MemoryUpdater::DeviceId).asScoped().withOption<Core::DeviceMemoryUpdateOptions>(
            [](InjectionContext context)
            {
                return new Core::DeviceMemoryUpdateOptions(
                        context.container.resolve<Core::Queue>(Constants::Queue::MainThreadTransferId),
                        context.container.resolve<Core::Fence>(),
                        context.container.resolve<Core::CommandPool>(Constants::CommandPool::MainThreadTransferPoolId));
            });;
    diContainer.bind<Core::Swapchain>().toSelf().asLazySingleton();
    diContainer.bind<Core::Buffer>().toSelf().asScoped();
    diContainer.bind<Core::Image>().toSelf().asScoped();
    diContainer.bind<Core::BufferView>().toSelf().asScoped();
    diContainer.bind<Core::ImageView>().toSelf().asScoped();
    diContainer.bind<Core::Sampler>().toSelf().asScoped();
    diContainer.bind<Core::ShaderModule>().toSelf().asScoped();
    diContainer.bind<Core::DescriptorSetLayout>().toSelf().asScoped();
    diContainer.bind<Core::DescriptorSet>().toSelf().asScoped();
    diContainer.bind<Core::DescriptorPool>().toSelf().asScoped();
    diContainer.bind<RenderTarget>().toSelf().asScoped();
    diContainer.bind<Core::RenderPass>().toSelf().asScoped();
    diContainer.bind<Core::Framebuffer>().toSelf().asScoped();
    diContainer.bind<BufferPool>().toSelf().asScoped();
    diContainer.bind<FencePool>().toSelf().asScoped();
    diContainer.bind<SemaphorePool>().toSelf().asScoped();
    diContainer.bind<RenderFrame>().toSelf().asScoped();
    diContainer.bind<ResourceCache>().toSelf().asLazySingleton();
    diContainer.bind<Core::PipelineLayout>().toSelf().asScoped();
    diContainer.bind<Core::GraphicsPipeline>().toSelf().asScoped();
    diContainer.bind<Core::ComputePipeline>().toSelf().asScoped();
    diContainer.bind<RenderContext>().toSelf().asLazySingleton();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("png")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("jpg")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("jpeg")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("bmp")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("tga")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("ppm")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("pgm")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("pic")).asScoped();
    diContainer.bind<ImageLoader>().to<BitmapImageLoader>(ResolveId::make<std::string>("hdr")).asScoped();
    diContainer.bind<ImageLoader>().to<CompressedImageLoader>(ResolveId::make<std::string>("ktx")).asScoped();
    diContainer.bind<ImageLoader>().to<CompressedImageLoader>(ResolveId::make<std::string>("dds")).asScoped();
    diContainer.bind<GeometrySubpass>().toSelf().asScoped();
    diContainer.bind<LightSubpass>().toSelf().asScoped();
    diContainer.bind<Light>().toSelf().asScoped();
    diContainer.bind<SpecularMeshRenderer>().toSelf().asScoped();
    diContainer.bind<ForwardLightsRenderer>().toSelf().asScoped();
    diContainer.bind<ForwardSpecularLightsRenderer>().toSelf().asScoped();
    diContainer.bind<SkyboxRenderer>().toSelf().asScoped();
    diContainer.bind<ParticleRenderer>().toSelf().asScoped();
    diContainer.bind<CubicBSplineRenderer>().toSelf().asScoped();
    diContainer.bind<DeferredRenderer>().toSelf().asScoped();
    diContainer.bind<DeferredSpecularRenderer>().toSelf().asScoped();
    diContainer.bind<BasicLightRenderer>().toSelf().asScoped();
    diContainer.bind<SpecularLightRenderer>().toSelf().asScoped();

    splashScreen = LavaVk::make<LoadingScreenLoader>();
}


void LavaVk::Application::installConfigurations(LavaVk::DIContainer &diContainer)
{
    diContainer.addOption<ApplicationOption>(
            []()
            {
                auto options = new ApplicationOption();
                options->appName = "LavaVk Application";
                options->version = {1, 0, 0};
#ifdef NDEBUG
                options->enableDebug = false;
#else
                options->enableDebug = true;
#endif
                options->additionalRequiredExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
                return options;
            });

    diContainer.addOption<WindowOptions>(
            []()
            {
                auto options = new WindowOptions();
                options->width = 800;
                options->height = 600;
                options->fullscreenMode = false;
                return options;
            });

    diContainer.addOption<Core::DeviceOptions>([]()
                                               { return new Core::DeviceOptions(); });
    diContainer.addOption<Core::AllocatorOptions>(
            [](const InjectionContext &context)
            {
                auto images = context.container.resolve<Core::Swapchain>()->getImages().size();
                return new Core::AllocatorOptions(images);
            });
    diContainer.addOption<Core::SwapchainOptions>([]()
                                                  { return new Core::SwapchainOptions(); });
    diContainer.addOption<RenderTargetOptions>([]()
                                               { return new DefaultRenderTargetOptions(); });
    diContainer.addOption<ResourceCacheOptions>(
            [](const InjectionContext &context)
            {
                auto images = context.container.resolve<Core::Swapchain>()->getImages().size();
                return new ResourceCacheOptions(images);
            });
    diContainer.addOption<MultisamplingOptions>([]()
                                                { return new MultisamplingOptions(); });
    diContainer.addOption<MipmapsOptions>([]()
                                          {
                                              return new MipmapsOptions(std::numeric_limits<uint32_t>::max(),
                                                                        LevelOfDetail::VeryHigh);
                                          });
}

LavaVk::Window &LavaVk::Application::getWindow()
{
    return *window;
}

void LavaVk::Application::initialize()
{
    logger = container.resolve<Logger>();
    timer = container.resolve<Timer>();
    time = container.resolve<Time>();
    frameCounter = container.resolve<FrameCounter>();
    window = container.resolve<Window>();
    SharedRenderPipeline pipeline;
    SharedRenderTargetOptions renderTargetOptions;
    {
        auto cmd = container.resolve<Core::CommandBuffer>(Constants::CommandBuffer::MainThreadTransferId);
        auto token = cmd->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        std::tie(activeScene, pipeline) = splashScreen->load(cmd);
    }
    updateResources(true);
    splashScreen->onSceneLoaded(container);
    renderContext = container.resolve<RenderContext>();
    renderContext->prepare();
    container.rebind<Scene>().toSelf().asSingleton(activeScene);
    container.bind<RenderPipeline>().toSelf().asSingleton(pipeline);
    initializeScene();
    loadSceneAsync(startScene());
    timer->start();
}

void LavaVk::Application::step()
{
    auto deltaTime = static_cast<float>(timer->tick<Timer::Seconds>());
    time->update(deltaTime);

    if (window->isFocused())
    {
        updateScene(deltaTime);
        update(deltaTime);
        draw();
        frameCounter->nextFrame(deltaTime);
    }

    updateActiveScene(sceneLoader);
}

void LavaVk::Application::mainLoop()
{
    while (!window->shouldClose())
    {
        step();
        window->processEvents();
    }

    container.resolve<Core::Device>()->waitIdle();
}

void LavaVk::Application::run()
{
    initialize();
    mainLoop();
    finish();
}

void LavaVk::Application::close()
{
    window->close();
}

void LavaVk::Application::finish()
{
    instance.reset();
}

void LavaVk::Application::updateScene(float deltaTime)
{
    if (activeScene)
    {
        //Update scripts
        if (activeScene->hasComponent<Script>())
        {
            auto scripts = activeScene->getComponents<Script>();
            for (auto &script : scripts)
                script->update(deltaTime);
        }
    }
}

void LavaVk::Application::draw()
{
    auto commandBuffer = renderContext->begin();

    {
        auto beginToken = commandBuffer->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        auto renderPipeline = container.resolve<RenderPipeline>();
        renderPipeline->prepare(commandBuffer);

        auto renderTarget = renderContext->getActiveFrame()->getRenderTarget();
        auto &views = renderTarget->getViews();
        uint32_t presentIndex = 0;

        auto barrier = commandBuffer->pipelineBarrier();

        for (size_t i = 0; i < views.size(); ++i)
        {
            if (isDepthStencilFormat(views[i]->getFormat()))
            {
                barrier.addImageMemoryBarrier(views[i]->getImage()->getHandle(),
                                              vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                                              vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                                              vk::PipelineStageFlagBits::eLateFragmentTests,
                                              vk::PipelineStageFlagBits::eLateFragmentTests,
                                              vk::ImageLayout::eUndefined,
                                              vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                              VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                              views[i]->getSubresourceRange().aspectMask);
            }
            else
            {
                barrier.addImageMemoryBarrier(views[i]->getImage()->getHandle(),
                                              vk::AccessFlagBits::eColorAttachmentWrite,
                                              vk::AccessFlagBits::eColorAttachmentWrite,
                                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                              vk::ImageLayout::eUndefined,
                                              vk::ImageLayout::eColorAttachmentOptimal,
                                              VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                              views[i]->getSubresourceRange().aspectMask);

                if (std::dynamic_pointer_cast<Core::SwapchainImage>(views[i]->getImage()))
                    presentIndex = i;
            }
        }

        barrier.execute();

        {
            auto extent = renderTarget->getExtent();

            vk::Viewport viewport{};
            viewport.width = static_cast<float>(extent.width);
            viewport.height = static_cast<float>(extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            commandBuffer->setViewport(0, {viewport});

            vk::Rect2D scissor{};
            scissor.extent = extent;
            commandBuffer->setScissor(0, {scissor});

            auto beginRenderToken = renderPipeline->draw(commandBuffer, renderTarget);
        }

        commandBuffer->pipelineBarrier()
                .addImageMemoryBarrier(views.at(presentIndex)->getImage()->getHandle(),
                                       vk::AccessFlagBits::eColorAttachmentWrite,
                                       {},
                                       vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                       vk::PipelineStageFlagBits::eBottomOfPipe,
                                       vk::ImageLayout::eColorAttachmentOptimal,
                                       vk::ImageLayout::ePresentSrcKHR,
                                       VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                       views.at(presentIndex)->getSubresourceRange().aspectMask)
                .execute();

    }

    renderContext->submit(commandBuffer);
}

void LavaVk::Application::initializeScene()
{
    if (activeScene)
    {
        //Update scripts
        if (activeScene->hasComponent<Script>())
        {
            auto scripts = activeScene->getComponents<Script>();
            for (auto &script : scripts)
                script->initialize(container);
        }
    }
}

void LavaVk::Application::loadSceneAsync(const LavaVk::SharedSceneLoader &sc)
{
    if (isSceneLoadingActive)
    {
        logger->warning("Another scene loading in progress!");
        return;
    }

    logger->information("Loading scene...");
    isSceneLoadingActive = true;
    auto cmd = container.resolve<Core::CommandBuffer>(Constants::CommandBuffer::MainThreadTransferId);
    sceneLoaderRef = sc;
    sceneLoader = std::async(std::launch::async,
                             [sc, cmd]()
                             {
                                 auto token = cmd->begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
                                 return sc->load(cmd);
                             });
}

bool LavaVk::Application::isLoadingScene() const
{
    return isSceneLoadingActive;
}

void LavaVk::Application::updateActiveScene(std::future<std::pair<SharedScene, SharedRenderPipeline>> &sc)
{
    if (isSceneLoadingActive)
    {
        if (sc.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            if (!isFenceWaitingActive)
                updateResources(false);

            if (fenceAwaiter.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                isFenceWaitingActive = false;
                container.resolve<Core::Device>()->waitIdle();
                SharedRenderPipeline pipeline{};
                SharedRenderTargetOptions renderTargetOptions;
                std::tie(activeScene, pipeline) = sc.get();
                sceneLoaderRef->onSceneLoaded(container);
                container.rebind<RenderPipeline>().toSelf().asSingleton(pipeline);
                container.rebind<Scene>().toSelf().asSingleton(activeScene);
                initializeScene();
                sc = {};
                sceneLoaderRef = {};
                isSceneLoadingActive = false;
            }
        }
    }
}

void LavaVk::Application::updateResources(bool waitOnMainThread)
{
    auto queue = container.resolve<Core::Queue>(Constants::Queue::MainThreadTransferId);
    auto cmd = container.resolve<Core::CommandBuffer>(Constants::CommandBuffer::MainThreadTransferId);
    auto fence = container.resolve<Core::Fence>();
    queue->createSubmitRequest().setFence(fence->getHandle()).submitInfo().addCommandBuffer(cmd->getHandle()).add().submit();
    if (waitOnMainThread)
        fence->wait();
    else
    {
        if (isFenceWaitingActive)
        {
            logger->warning("Another resource updating already in progress!");
            return;
        }
        isFenceWaitingActive = true;
        fenceAwaiter = std::async(std::launch::async, [fence]()
        { fence->wait(); });
    }
}
