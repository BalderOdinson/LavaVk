//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_RENDERFRAME_H
#define LAVAVK_RENDERFRAME_H

#include <vector>
#include <map>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/core/queue.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/device.h"
#include "lava/framework/core/commandpool.h"
#include "lava/framework/core/descriptorpool.h"
#include "lava/framework/core/descriptorset.h"
#include "lava/framework/fencepool.h"
#include "lava/framework/semaphorepool.h"
#include "lava/framework/bufferpool.h"
#include "rendertarget.h"
#include "lava/framework/core/options/descriptorsetoptions.h"
#include "lava/framework/core/options/descriptorpooloptions.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk
{
    struct CommandPoolIndex : Object
    {
        CommandPoolIndex(uint32_t queueFamilyIndex, Core::CommandBuffer::ResetMode resetMode);

        uint32_t queueFamilyIndex;
        Core::CommandBuffer::ResetMode resetMode;

        bool equals(const Object &other) const override;
        size_t getHashCode() const override;

        std::type_index getType() const override;
    };

    /// \brief RenderFrame is a container for per-frame data.
    class RenderFrame : public virtual Object
    {
    public:
        /// \brief Block size of a buffer pool in kilobytes
        static constexpr uint32_t BUFFER_POOL_BLOCK_SIZE = 256;

        explicit RenderFrame(InjectionContext &context);
        RenderFrame(const RenderFrame &) = delete;
        RenderFrame(RenderFrame &&) = delete;

        RenderFrame &operator=(const RenderFrame &) = delete;
        RenderFrame &operator=(RenderFrame &&) = delete;

        void reset();

        SharedFencePool getFencePool() const;
        Core::SharedFence requestFence(Core::FenceState desiredState = Core::FenceState::Unsignaled);
        SharedSemaphorePool getSemaphorePool() const;
        Core::SharedSemaphore requestSemaphore();

        void updateRenderTarget(SharedRenderTarget target);
        SharedRenderTarget getRenderTarget() const;

        Core::SharedCommandBuffer
        requestCommandBuffer(const Core::SharedQueue &queue,
                             Core::CommandBuffer::ResetMode resetMode = Core::CommandBuffer::ResetMode::ResetPool,
                             vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary, size_t threadIdx = 0);

        Core::SharedDescriptorSet
        requestDescriptorSet(const Core::SharedDescriptorSetOptions& descriptorSetOptions, size_t threadIdx);

        void clearDescriptors();

        BufferAllocation allocateBuffer(vk::BufferUsageFlagBits usage, vk::DeviceSize size, size_t threadIdx = 0);

        std::type_index getType() const override;

    private:
        std::vector<Core::SharedCommandPool> &
        getCommandPools(const Core::SharedQueue &queue, Core::CommandBuffer::ResetMode resetMode);

        Core::SharedDevice device;
        SharedLogger logger;
        /// Commands pools associated to the frame
        std::unordered_map<CommandPoolIndex, std::vector<Core::SharedCommandPool>, hash<CommandPoolIndex>, ObjectEquality<CommandPoolIndex>> commandPools;
        /// Descriptor pools for the frame
        std::vector<std::unordered_map<
                Core::SharedDescriptorSetLayout, // key
                Core::SharedDescriptorPool, // value
                hash<Core::SharedDescriptorSetLayout>, // hash function
                SharedObjectEquality<Core::DescriptorSetLayout> // equality comparator
        >> descriptorPools;
        /// Descriptor sets for the frame
        std::vector<std::unordered_map<
                Core::SharedDescriptorSetOptions, // key
                Core::SharedDescriptorSet,  // value
                hash<Core::SharedDescriptorSetOptions>, // hash function
                SharedObjectEquality<Core::DescriptorSetOptions> // equality comparator
        >> descriptorSets;
        SharedFencePool fencePool;
        SharedSemaphorePool semaphorePool;
        size_t threadCount;
        SharedRenderTarget renderTarget;
        std::map<vk::BufferUsageFlagBits, std::vector<std::pair<SharedBufferPool, SharedBufferBlock>>> bufferPools;
    };

    using UniqueRenderFrame = std::unique_ptr<RenderFrame>;
    using SharedRenderFrame = std::shared_ptr<RenderFrame>;
    using WeakRenderFrame = std::weak_ptr<RenderFrame>;
}


#endif //LAVAVK_RENDERFRAME_H
