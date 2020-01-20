//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_COMMANDPOOLOPTIONS_H
#define LAVAVK_COMMANDPOOLOPTIONS_H

#include <numeric>
#include "lava/framework/object.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/rendering/renderframe.h"

namespace LavaVk::Core
{
    class CommandPoolOptions : public virtual Object
    {
    public:
        explicit CommandPoolOptions(CommandBuffer::ResetMode resetMode, bool releaseOnReset, uint32_t queueFamilyIndex,
                                    uint32_t threadIndex, SharedRenderFrame frame = nullptr);

        CommandBuffer::ResetMode resetMode;
        uint32_t queueFamilyIndex;
        /// Set to true if pool should release resources on reset.
        bool releaseOnReset;
        uint32_t threadIndex;
        SharedRenderFrame frame;

        std::type_index getType() const override;
    };
}

#endif //LAVAVK_COMMANDPOOLOPTIONS_H
