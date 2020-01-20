//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_FRAMEBUFFEROPTIONS_H
#define LAVAVK_FRAMEBUFFEROPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/rendering/rendertarget.h"
#include "lava/framework/core/renderpass.h"

namespace LavaVk::Core
{
    class FramebufferOptions : public virtual Object
    {
    public:
        FramebufferOptions() = default;
        FramebufferOptions(SharedRenderTarget renderTarget, SharedRenderPass renderPass);

        SharedRenderTarget renderTarget;
        SharedRenderPass renderPass;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniqueFramebufferOptions = std::unique_ptr<FramebufferOptions>;
    using SharedFramebufferOptions = std::shared_ptr<FramebufferOptions>;
    using WeakFramebufferOptions = std::weak_ptr<FramebufferOptions>;
}


#endif //LAVAVK_FRAMEBUFFEROPTIONS_H
