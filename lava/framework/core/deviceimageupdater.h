//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_DEVICEIMAGEUPDATER_H
#define LAVAVK_DEVICEIMAGEUPDATER_H

#include "imageupdater.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"
#include "commandpool.h"
#include "commandbuffer.h"
#include "queue.h"
#include "imageobject.h"
#include "allocator.h"
#include "imageupdater.h"
#include "fence.h"
#include "bufferupdater.h"

namespace LavaVk::Core
{
    class DeviceImageUpdater : public ImageUpdater
    {
    public:
        explicit DeviceImageUpdater(InjectionContext &context);

        void update(SharedImageObject dst, void *data, vk::DeviceSize size, uint32_t mipLevel, uint32_t baseArrayLayer,
                    uint32_t layerCount, vk::Offset3D imageOffset,
                    vk::Extent3D imageExtent, const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
                    const vk::PipelineStageFlags &generatingStages, const vk::PipelineStageFlags &consumingStages,
                    vk::ImageLayout currentLayout, vk::ImageLayout newLayout,
                    const vk::ImageAspectFlags &aspect) override;
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


#endif //LAVAVK_DEVICEIMAGEUPDATER_H
