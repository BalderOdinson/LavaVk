//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_RENDERFRAMEOPTIONS_H
#define LAVAVK_RENDERFRAMEOPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/rendering/rendertarget.h"

namespace LavaVk
{
    class RenderFrameOptions : public Object
    {
    public:
        RenderFrameOptions() = default;
        explicit RenderFrameOptions(SharedRenderTarget renderTarget, size_t threadCount = 1);

        SharedRenderTarget renderTarget;
        size_t threadCount{1};

        std::type_index getType() const override;
    };
}

#endif //LAVAVK_RENDERFRAMEOPTIONS_H
