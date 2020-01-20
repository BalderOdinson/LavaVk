//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_BUFFERPOOL_H
#define LAVAVK_BUFFERPOOL_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/buffer.h"

namespace LavaVk
{
    /// \brief An allocation of vulkan memory; different buffer allocations,
    /// with different offset and size, may come from the same Vulkan buffer
    class BufferAllocation
    {
    public:
        BufferAllocation() = default;
        explicit BufferAllocation(Core::SharedBuffer buffer, vk::DeviceSize size, vk::DeviceSize offset);
        BufferAllocation(const BufferAllocation &) = delete;
        BufferAllocation(BufferAllocation &&) = default;

        BufferAllocation &operator=(const BufferAllocation &) = delete;
        BufferAllocation &operator=(BufferAllocation &&) = default;

        void
        update(const std::vector<uint8_t> &data, vk::DeviceSize offset = 0, const vk::AccessFlags &currentAccess = {},
               const vk::AccessFlags &newAccess = {}, const vk::PipelineStageFlags &generatingStages = {},
               const vk::PipelineStageFlags &consumingStages = {});

        template<class T>
        void update(const T &value, vk::DeviceSize offset = 0, const vk::AccessFlags &currentAccess = {},
                    const vk::AccessFlags &newAccess = {}, const vk::PipelineStageFlags &generatingStages = {},
                    const vk::PipelineStageFlags &consumingStages = {})
        {
            update(std::vector<uint8_t>{reinterpret_cast<const uint8_t *>(&value),
                                        reinterpret_cast<const uint8_t *>(&value) + sizeof(T)},
                   offset);
        }

        [[nodiscard]] bool empty() const;
        [[nodiscard]] vk::DeviceSize getSize() const;
        [[nodiscard]] vk::DeviceSize getOffset() const;
        Core::SharedBuffer &getBuffer();

    private:
        Core::SharedBuffer buffer;
        vk::DeviceSize baseOffset{0};
        vk::DeviceSize size{0};
    };

    /// \brief Helper class which handles multiple allocation from the same underlying Vulkan buffer.
    class BufferBlock : public virtual Object
    {
    public:
        BufferBlock(vk::DeviceSize size, const vk::BufferUsageFlags &usage, vma::MemoryUsage memoryUsage);

        /// \return An usable view on a portion of the underlying buffer
        BufferAllocation allocate(uint32_t size);
        [[nodiscard]] vk::DeviceSize getSize() const;

        void reset();

        std::type_index getType() const override;

    private:
        Core::SharedBuffer buffer;
        // Memory alignment, it may change according to the usage
        vk::DeviceSize alignment{0};
        // Current offset, it increases on every allocation
        vk::DeviceSize offset{0};
    };

    using UniqueBufferBlock = std::unique_ptr<BufferBlock>;
    using SharedBufferBlock = std::shared_ptr<BufferBlock>;
    using WeakBufferBlock = std::weak_ptr<BufferBlock>;


    /// @brief A pool of buffer blocks for a specific usage.
    /// It may contain inactive blocks that can be recycled.
    class BufferPool : public virtual Object
    {
    public:
        explicit BufferPool(InjectionContext &context);
        BufferPool(const BufferPool &) = delete;
        BufferPool(BufferPool &&) = delete;

        BufferPool &operator=(const BufferPool &) = delete;
        BufferPool &operator=(BufferPool &&) = delete;

        SharedBufferBlock requestBufferBlock(vk::DeviceSize minimumSize);

        void reset();

        std::type_index getType() const override;

    private:
        Core::SharedDevice device;
        SharedLogger logger;
        /// List of blocks requested
        std::vector<SharedBufferBlock> bufferBlocks;
        /// Minimum size of the blocks
        vk::DeviceSize blockSize{0};
        vk::BufferUsageFlags usage{};
        vma::MemoryUsage memoryUsage{};

        /// Numbers of active blocks from the start of buffer_blocks
        uint32_t activeBufferBlockCount{0};
    };

    using UniqueBufferPool = std::unique_ptr<BufferPool>;
    using SharedBufferPool = std::shared_ptr<BufferPool>;
    using WeakBufferPool = std::weak_ptr<BufferPool>;
}


#endif //LAVAVK_BUFFERPOOL_H
