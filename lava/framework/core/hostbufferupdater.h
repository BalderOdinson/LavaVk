//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_HOSTBUFFERUPDATER_H
#define LAVAVK_HOSTBUFFERUPDATER_H

#include "bufferupdater.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"

namespace LavaVk::Core
{
    class HostBufferUpdater : public BufferUpdater
    {
    public:
        void update(SharedBuffer buffer, void *data, size_t size, size_t offset, const vk::AccessFlags &currentAccess,
                    const vk::AccessFlags &newAccess, const vk::PipelineStageFlags &generatingStages,
                    const vk::PipelineStageFlags &consumingStages) override;
        std::type_index getType() const override;
    };
}


#endif //LAVAVK_HOSTBUFFERUPDATER_H
