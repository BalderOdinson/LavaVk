//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_DEVICEMEMORYUPDATEOPTIONS_H
#define LAVAVK_DEVICEMEMORYUPDATEOPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/core/queue.h"
#include "lava/framework/core/fence.h"
#include "lava/framework/core/commandpool.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk::Core
{
    class DeviceMemoryUpdateOptions : public virtual Object
    {
    public:
        DeviceMemoryUpdateOptions() = default;
        DeviceMemoryUpdateOptions(SharedQueue queue, SharedFence fence, SharedCommandPool pool);
        DeviceMemoryUpdateOptions(SharedCommandBuffer buffer);

        SharedQueue queue;
        SharedFence fence;
        SharedCommandPool pool;
        SharedCommandBuffer commandBuffer;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_DEVICEMEMORYUPDATEOPTIONS_H
