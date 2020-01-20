//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BUFFERVIEW_H
#define LAVAVK_BUFFERVIEW_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "buffer.h"
#include "options/bufferviewoptions.h"
#include "lava/framework/event-system/event.h"

namespace LavaVk::Core
{
    class BufferView : public virtual Object
    {
    public:
        explicit BufferView(InjectionContext &context);
        BufferView(BufferViewOptions options);
        BufferView(const BufferView &) = delete;
        BufferView(BufferView &&) = delete;
        ~BufferView() override;

        BufferView &operator=(const BufferView &) = delete;
        BufferView &operator=(BufferView &&) = delete;

        SharedBuffer getBuffer() const;

        vk::BufferView getHandle() const;

        vk::Format getFormat() const;
        vk::DeviceSize getSize() const;
        vk::DeviceSize getSizeInBytes() const;
        vk::DeviceSize getOffset() const;

        std::type_index getType() const override;

    private:
        vk::BufferView handle{};
        SharedDevice device{};
        BufferViewOptions options{};
        EventToken rebindToken{};
        EventToken recreateToken{};

        void recreate(Object& buffer);
    };

    using UniqueBufferView = std::unique_ptr<BufferView>;
    using SharedBufferView = std::shared_ptr<BufferView>;
    using WeakBufferView = std::weak_ptr<BufferView>;
}


#endif //LAVAVK_BUFFERVIEW_H
