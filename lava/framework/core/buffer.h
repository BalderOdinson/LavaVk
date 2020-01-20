//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_BUFFER_H
#define LAVAVK_BUFFER_H

#include <vulkan/vulkan.hpp>
#include "lava/third-party/vma.h"
#include "allocation.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "options/bufferoptions.h"
#include "lava/framework/utils.h"
#include "lava/framework/event-system/event.h"
#include "bufferupdater.h"

namespace LavaVk::Core
{
    class Buffer : public Allocation
    {
    public:
        explicit Buffer(InjectionContext &context);
        explicit Buffer(BufferOptions options);
        Buffer(const Buffer &) = delete;
        Buffer(Buffer &&) = delete;
        ~Buffer() override;

        Buffer &operator=(const Buffer &) = delete;
        Buffer &operator=(Buffer &&) = delete;

        vk::Buffer getHandle() const;

        Event<EventHandler<>> &rebonded();
        Event<EventHandler<>> &recreated();

        [[nodiscard]] vk::BufferUsageFlags getUsage() const;

        [[nodiscard]] vk::DeviceSize getSize() const;

        vma::Allocation getAllocation() const override;
        SharedAllocator getAllocator() const override;
        void recreate() override;
        void rebind() override;
        std::type_index getType() const override;

        void update(void *data, vk::DeviceSize size, vk::DeviceSize offset, const vk::AccessFlags &currentAccess,
                    const vk::AccessFlags &newAccess, const vk::PipelineStageFlags &generatingStages,
                    const vk::PipelineStageFlags &consumingStages);

    protected:
        void getData(void *data, size_t *size) const override;

    private:
        SharedAllocator allocator;
        vk::Buffer handle;
        vk::DeviceSize size;
        vma::Allocation allocationHandle;
        BufferOptions options;
        EventOwner<EventHandler<>> rebondedEventSubject;
        EventOwner<EventHandler<>> recreatedChangedEventSubject;
    };

    using UniqueBuffer = std::unique_ptr<Buffer>;
    using SharedBuffer = std::shared_ptr<Buffer>;
    using WeakBuffer = std::weak_ptr<Buffer>;
}


#endif //LAVAVK_BUFFER_H
