//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BUFFERUPDATER_H
#define LAVAVK_BUFFERUPDATER_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class Buffer;

    using UniqueBuffer = std::unique_ptr<Buffer>;
    using SharedBuffer = std::shared_ptr<Buffer>;
    using WeakBuffer = std::weak_ptr<Buffer>;

    class BufferUpdater : public virtual Object
    {
    public:
        virtual void update(SharedBuffer buffer, void *data, size_t size, size_t offset, const vk::AccessFlags &currentAccess,
                    const vk::AccessFlags &newAccess, const vk::PipelineStageFlags &generatingStages,
                    const vk::PipelineStageFlags &consumingStages) = 0;
    };

    using UniqueBufferUpdater = std::unique_ptr<BufferUpdater>;
    using SharedBufferUpdater = std::shared_ptr<BufferUpdater>;
    using WeakBufferUpdater = std::weak_ptr<BufferUpdater>;
}


#endif //LAVAVK_BUFFERUPDATER_H
