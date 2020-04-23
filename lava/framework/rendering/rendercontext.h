//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_RENDERCONTEXT_H
#define LAVAVK_RENDERCONTEXT_H

#include <vulkan/vulkan.hpp>
#include <lava/framework/core/swapchain.h>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/core/semaphore.h"
#include "lava/framework/core/queue.h"
#include "renderframe.h"
#include "lava/framework/platform/window.h"
#include "lava/framework/event-system/event.h"
#include "lava/framework/threadingoptions.h"

namespace LavaVk
{
    /// \brief RenderContext acts as a frame manager for the sample, with a lifetime that is the
    /// same as that of the Application itself. It acts as a container for RenderFrame objects,
    /// swapping between them (begin_frame, end_frame) and forwarding requests for Vulkan resources
    /// to the active frame. Note that it's guaranteed that there is always an active frame.
    /// More than one frame can be in-flight in the GPU, thus the need for per-frame resources.
    class RenderContext : public virtual Object
    {
    public:
        explicit RenderContext(InjectionContext &context);
        RenderContext(const RenderContext &) = delete;
        RenderContext(RenderContext &&) = delete;
        ~RenderContext() override;

        RenderContext &operator=(const RenderContext &) = delete;
        RenderContext &operator=(RenderContext &&) = delete;

        /// \brief Prepares the RenderFrames for rendering
        void prepare();

        /// \brief Recreates the RenderFrames, called after every update
        void recreate();

        /// \brief Prepares the next available frame for rendering
        /// \param resetMode How to reset the command buffer
        /// \returns A valid command buffer to record commands to be submitted
        Core::SharedCommandBuffer
        begin(Core::CommandBuffer::ResetMode resetMode = Core::CommandBuffer::ResetMode::ResetPool);

        /// \brief Submits the command buffer to the right queue
        /// \param commandBuffer A command buffer containing recorded commands
        void submit(Core::SharedCommandBuffer &commandBuffer);

        /// \brief beginFrame
        /// \return VkSemaphore
        Core::SharedSemaphore beginFrame();

        Core::SharedSemaphore submit(const Core::SharedQueue &queue, const Core::SharedCommandBuffer &CommandBuffer,
                                     const Core::SharedSemaphore& waitSemaphore, const vk::PipelineStageFlags& waitPipelineStage);

        /// \brief Submits a command buffer related to a frame to a queue
        void submit(const Core::SharedQueue &queue, const Core::SharedCommandBuffer &commandBuffer);

        /// \brief Waits a frame to finish its rendering
        void waitFrame();

        void endFrame(Core::SharedSemaphore semaphore);

        /// \brief An error should be raised if the frame is not active.
        /// A frame is active after \ref beginFrame has been called.
        /// return The current active frame
        const SharedRenderFrame &getActiveFrame() const;

        /// \brief An error should be raised if the frame is not active.
        /// A frame is active after @ref begin_frame has been called.
        /// \return The current active frame index
        uint32_t getActiveFrameIndex() const;

        /// \brief An error should be raised if a frame is active.
        /// A frame is active after @ref begin_frame has been called.
        /// \return The previous frame
        const SharedRenderFrame &getLastRenderedFrame() const;

        const SharedRenderFrame &getRenderFrame(uint32_t index) const;

        const Core::SharedQueue &getRenderQueue() const;

        Core::SharedSemaphore requestSemaphore();

	    /// \brief Handles surface changes, only applicable if the render_context makes use of a swapchain
        virtual void handleSurfaceChanges();

        std::type_index getType() const override;

    private:
        SharedWindow window;
        Core::SharedDevice device;
        SharedLogger logger;
        Core::SharedAllocator allocator;
        SharedResourceCache resourceCache;
        Core::SharedQueue queue;
        Core::SharedSwapchain swapchain;
        std::vector<SharedRenderFrame> frames;
        Core::SharedSemaphore acquiredSemaphore;
        vk::Extent2D surfaceExtent;
        bool prepared{false};
        /// Current active frame index
        uint32_t activeFrameIndex{0};
        /// Whether a frame is active or not
        bool frameActive{false};
        vk::SurfaceTransformFlagBitsKHR preTransform{vk::SurfaceTransformFlagBitsKHR::eIdentity};
        SharedThreadingOptions threadingOptions;
        EventToken swapchainRecreatedToken;
    };

    using UniqueRenderContext = std::unique_ptr<RenderContext>;
    using SharedRenderContext = std::shared_ptr<RenderContext>;
    using WeakRenderContext = std::weak_ptr<RenderContext>;
}


#endif //LAVAVK_RENDERCONTEXT_H
