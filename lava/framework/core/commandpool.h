//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_COMMANDPOOL_H
#define LAVAVK_COMMANDPOOL_H

#include <vector>
#include <atomic>
#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "commandbuffer.h"
#include "device.h"

namespace LavaVk
{
    class RenderFrame;

    using UniqueRenderFrame = std::unique_ptr<RenderFrame>;
    using SharedRenderFrame = std::shared_ptr<RenderFrame>;
    using WeakRenderFrame = std::weak_ptr<RenderFrame>;
}

namespace LavaVk::Core
{
    class CommandPool : public virtual Object
    {
    public:
        friend class CommandBuffer;

        explicit CommandPool(InjectionContext &context);
        CommandPool(const CommandPool &) = delete;
        CommandPool(CommandPool &&) = delete;
        ~CommandPool() override;

        CommandPool &operator=(const CommandPool &) = delete;
        CommandPool &operator=(CommandPool &&) = delete;

        SharedCommandBuffer requestBuffer(vk::CommandBufferLevel level);
        vk::CommandPool getHandle() const;
        void reset();
        void resetCommandBuffers();
        std::type_index getType() const override;
        CommandBuffer::ResetMode getResetMode() const;
        uint32_t getQueueFamilyIndex() const;
        uint32_t getThreadIndex() const;
        SharedRenderFrame getFrame() const;

    private:
        vk::CommandPool handle;
        SharedDevice device;
        uint32_t threadIndex;
        WeakRenderFrame frame;
        std::vector<SharedCommandBuffer> allocatedPrimaryBuffers;
        std::vector<SharedCommandBuffer> allocatedSecondaryBuffers;
        size_t activePrimaryBuffersCount{0};
        size_t activeSecondaryBuffersCount{0};
        CommandBuffer::ResetMode resetMode;
        uint32_t queueFamilyIndex{0};
        bool releaseOnReset;
    };

    using UniqueCommandPool = std::unique_ptr<CommandPool>;
    using SharedCommandPool = std::shared_ptr<CommandPool>;
    using WeakCommandPool = std::weak_ptr<CommandPool>;

}

#endif //LAVAVK_COMMANDPOOL_H
