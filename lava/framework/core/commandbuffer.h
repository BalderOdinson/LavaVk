//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_COMMANDBUFFER_H
#define LAVAVK_COMMANDBUFFER_H

#include <vulkan/vulkan.hpp>
#include <queue>
#include "lava/framework/object.h"
#include "device.h"
#include "begintoken.h"
#include "renderpass.h"
#include "framebuffer.h"
#include "lava/framework/rendering/rendertarget.h"
#include "options/renderpassoptions.h"
#include "pipelinelayout.h"
#include "buffer.h"
#include "sampler.h"
#include "image.h"
#include "lava/framework/rendering/pipelinestate.h"
#include "buffertransfer.h"
#include "imagetransfer.h"
#include "image2buffertransfer.h"
#include "buffer2imagetransfer.h"
#include "lava/framework/resourcebindingstate.h"
#include "lava/framework/platform/logger.h"
#include "pipeline.h"
#include "lava/framework/multisamplingoptions.h"

namespace LavaVk
{
    class Subpass;

    using UniqueSubpass = std::unique_ptr<Subpass>;
    using SharedSubpass = std::shared_ptr<Subpass>;
    using WeakSubpass = std::weak_ptr<Subpass>;

    class ResourceCache;

    using UniqueResourceCache = std::unique_ptr<ResourceCache>;
    using SharedResourceCache = std::shared_ptr<ResourceCache>;
    using WeakResourceCache = std::weak_ptr<ResourceCache>;
}

namespace LavaVk::Core
{
    /// \brief Helper structure used to track render pass state
    struct RenderPassBinding
    {
        SharedRenderPass renderPass;
        SharedFramebuffer framebuffer;
    };

    class CommandPool;

    using UniqueCommandPool = std::unique_ptr<CommandPool>;
    using SharedCommandPool = std::shared_ptr<CommandPool>;
    using WeakCommandPool = std::weak_ptr<CommandPool>;

    class PipelineBarrier;

    class CommandBuffer : public virtual Object
    {
    public:
        friend class BeginToken;

        CommandBuffer(const SharedCommandPool &pool, vk::CommandBufferLevel level);

        CommandBuffer(const CommandBuffer &) = delete;

        CommandBuffer(CommandBuffer &&) = delete;

        ~CommandBuffer() override;

        CommandBuffer &operator=(const CommandBuffer &) = delete;

        CommandBuffer &operator=(CommandBuffer &&) = delete;

        enum class ResetMode
        {
            ResetIndividually,
            ResetPool
        };

        enum class State
        {
            Invalid,
            Initial,
            Recording,
            Executable,
        };

        vk::CommandBuffer getHandle();

        vk::CommandBufferLevel getLevel() const;

        void reset(ResetMode resetMode);

        State getState() const;

        bool isRecording() const;

        std::vector<uint8_t> storedPushConstants;

        /// \brief Sets the command buffer so that it is ready for recording
        ///        If it is a secondary command buffer, a pointer to the
        ///        primary command buffer it inherits from must be provided
        /// \param flags Usage behavior for the command buffer
        /// \param primaryBuffer (optional)
        /// \return Token which destruction ends recording.
        BeginToken begin(const vk::CommandBufferUsageFlags &flags, const SharedCommandBuffer &primaryBuffer = nullptr);

        void clear(const vk::ClearAttachment &info, vk::ClearRect rect);

        BeginRenderPassToken
        beginRenderPass(const SharedRenderTarget &renderTarget, const std::vector<LoadStoreInfo> &loadStoreInfos,
                        const std::vector<vk::ClearValue> &clearValues,
                        const std::vector<SharedSubpass> &subpasses,
                        vk::SubpassContents contents = vk::SubpassContents::eInline);

        void nextSubpass();

        void executeCommands(const SharedCommandBuffer &secondaryCommandBuffer);

        void executeCommands(const std::vector<SharedCommandBuffer> &secondaryCommandBuffers);

        void bindPipelineLayout(const SharedPipelineLayout &pipelineLayout);

        template<class T>
        void setSpecializationConstant(uint32_t constantId, const T &data);

        void setSpecializationConstant(uint32_t constantId, const std::vector<uint8_t> &data);

        /// \brief Stores additional data which is prepended to the
        ///        values passed to the pushConstant() function
        /// \param data Data to be stored
        template<class T>
        void setPushConstants(const T &data);

        void setPushConstants(const std::vector<uint8_t> &values);

        void pushConstants(uint32_t offset, const std::vector<uint8_t> &values);

        template<typename T>
        void pushConstants(uint32_t offset, const T &value)
        {
            pushConstants(offset,
                          std::vector<uint8_t>{reinterpret_cast<const uint8_t *>(&value),
                                               reinterpret_cast<const uint8_t *>(&value) + sizeof(T)});
        }

        void
        bindBuffer(const SharedBuffer &buffer, vk::DeviceSize offset, vk::DeviceSize range, uint32_t set,
                   uint32_t binding,
                   uint32_t arrayElement);

        void bindBufferView(const SharedBufferView &bufferView, uint32_t set, uint32_t binding, uint32_t arrayElement);

        void bindImage(const SharedImageView &imageView, const SharedSampler &sampler, uint32_t set, uint32_t binding,
                       uint32_t arrayElement);

        void bindInput(const SharedImageView &imageView, uint32_t set, uint32_t binding, uint32_t arrayElement);

        void bindVertexBuffers(uint32_t firstBinding,
                               const std::vector<SharedBuffer> &buffers,
                               const std::vector<vk::DeviceSize> &offsets);

        void bindIndexBuffer(const SharedBuffer &buffer, vk::DeviceSize offset, vk::IndexType indexType);

        void setViewportState(const ViewportState &stateInfo);

        void setVertexInputState(const VertexInputState &stateInfo);

        void setInputAssemblyState(const InputAssemblyState &stateInfo);

        void setRasterizationState(const RasterizationState &stateInfo);

        void setMultisampleState(const MultisampleState &stateInfo);

        void setDepthStencilState(const DepthStencilState &stateInfo);

        void setColorBlendState(const ColorBlendState &stateInfo);

        void setViewport(uint32_t firstViewport, const std::vector<vk::Viewport> &viewports);

        void setScissor(uint32_t firstScissor, const std::vector<vk::Rect2D> &scissors);

        void setLineWidth(float lineWidth);

        void setDepthBias(float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);

        void setBlendConstants(const std::array<float, 4> &blendConstants);

        void setDepthBounds(float minDepthBounds, float maxDepthBounds);

        void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);

        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                         uint32_t firstInstance);

        void
        drawIndexedIndirect(const SharedBuffer &buffer, vk::DeviceSize offset, uint32_t drawCount, uint32_t stride);

        void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

        void dispatchIndirect(const SharedBuffer &buffer, vk::DeviceSize offset);

        void updateBuffer(const SharedBuffer &buffer, vk::DeviceSize offset, const std::vector<uint8_t> &data);

        void
        blitImage(const SharedImage &srcImg, const SharedImage &dstImg, const std::vector<vk::ImageBlit> &regions);

        template<typename Src, typename Dst>
        auto copy(std::shared_ptr<Src> src, std::shared_ptr<Dst> dst);

        PipelineBarrier pipelineBarrier();

        std::type_index getType() const override;

        void saveExecutionResource(SharedObject object);
        void clearExecutionResources();

    private:
        SharedDevice device;
        SharedResourceCache cache;
        SharedLogger logger;
        WeakCommandPool pool;
        State state{State::Invalid};
        vk::CommandBuffer handle;
        vk::CommandBufferLevel level;
        RenderPassBinding currentRenderPass;
        PipelineState pipelineState;
        ResourceBindingState resourceBindingState;
        std::unordered_map<uint32_t, SharedDescriptorSetLayout> descriptorSetLayoutState;
        SharedMultisamplingOptions multisamplingOptions;
        std::vector<SharedObject> requiredExecutionResources;

        const RenderPassBinding &getCurrentRenderPass() const;

        uint32_t getCurrentSubpassIndex() const;

        /// \brief Flush the piplines state
        void flushPipelineState(vk::PipelineBindPoint pipelineBindPoint);

        /// \brief Flush the descriptor set state
        void flushDescriptorState(vk::PipelineBindPoint pipelineBindPoint);
    };

    using UniqueCommandBuffer = std::unique_ptr<CommandBuffer>;
    using SharedCommandBuffer = std::shared_ptr<CommandBuffer>;
    using WeakCommandBuffer = std::weak_ptr<CommandBuffer>;

    template<class T>
    inline void CommandBuffer::setPushConstants(const T &data)
    {
        setPushConstants(
                {reinterpret_cast<const uint8_t *>(&data),
                 reinterpret_cast<const uint8_t *>(&data) + sizeof(T)});
    }

    template<>
    inline void CommandBuffer::setPushConstants<bool>(const bool &data)
    {
        auto value = static_cast<uint32_t>(data);

        setPushConstants(
                {reinterpret_cast<const uint8_t *>(&value),
                 reinterpret_cast<const uint8_t *>(&value) + sizeof(std::uint32_t)});
    }

    template<class T>
    inline void CommandBuffer::setSpecializationConstant(uint32_t constantId, const T &data)
    {
        setSpecializationConstant(constantId,
                                  {reinterpret_cast<const uint8_t *>(&data),
                                   reinterpret_cast<const uint8_t *>(&data) + sizeof(T)});
    }

    template<>
    inline void CommandBuffer::setSpecializationConstant<bool>(std::uint32_t constantId, const bool &data)
    {
        auto value = static_cast<uint32_t >(data);

        setSpecializationConstant(
                constantId,
                {reinterpret_cast<const uint8_t *>(&value),
                 reinterpret_cast<const uint8_t *>(&value) + sizeof(std::uint32_t)});
    }

    template<typename Src, typename Dst>
    auto LavaVk::Core::CommandBuffer::copy(std::shared_ptr<Src> src, std::shared_ptr<Dst> dst)
    {
        if constexpr (std::is_base_of<Buffer, Src>::value)
        {
            if constexpr (std::is_base_of<Buffer, Dst>::value)
                return BufferTransfer(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()), std::move(src),
                                      std::move(dst));

            if constexpr (std::is_base_of<ImageObject, Dst>::value)
                return Buffer2ImageTransfer(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()),
                                            std::move(src),
                                            std::move(dst));
        }
        if constexpr (std::is_base_of<ImageObject, Src>::value)
        {
            if constexpr (std::is_base_of<Buffer, Dst>::value)
                return Image2BufferTransfer(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()),
                                            std::move(src),
                                            std::move(dst));

            if constexpr (std::is_base_of<ImageObject, Dst>::value)
                return ImageTransfer(std::dynamic_pointer_cast<CommandBuffer>(shared_from_this()), std::move(src),
                                     std::move(dst));
        }
    }
}


#endif //LAVAVK_COMMANDBUFFER_H
