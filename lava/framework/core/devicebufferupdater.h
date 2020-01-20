//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_DEVICEBUFFERUPDATER_H
#define LAVAVK_DEVICEBUFFERUPDATER_H

#include "bufferupdater.h"
#include "device.h"
#include "allocator.h"
#include "queue.h"
#include "commandpool.h"
#include "fence.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk::Core
{
    class DeviceBufferUpdater : public BufferUpdater
    {
    public:
        explicit DeviceBufferUpdater(InjectionContext &context);

        void update(SharedBuffer buffer, void *data, size_t size, size_t offset, const vk::AccessFlags &currentAccess,
                    const vk::AccessFlags &newAccess, const vk::PipelineStageFlags &generatingStages,
                    const vk::PipelineStageFlags &consumingStages) override;
        std::type_index getType() const override;

    private:
        SharedDevice device;
        SharedAllocator allocator;
        SharedQueue transferQueue;
        SharedCommandPool pool;
        SharedFence fence;
        SharedCommandBuffer commandBuffer;
        SharedBufferUpdater bufferUpdater;
    };
}


#endif //LAVAVK_DEVICEBUFFERUPDATER_H
