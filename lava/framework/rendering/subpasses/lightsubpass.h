//
// Created by oshikuru on 19. 01. 2020..
//

#ifndef DEFERRED_LIGHTSUBPASS_H
#define DEFERRED_LIGHTSUBPASS_H

#include "lava/framework/scene-graph/components/camera.h"
#include "lava/framework/rendering/subpass.h"
#include "lightrenderer.h"

namespace LavaVk
{
    class LightSubpass : public Subpass
    {
    public:
        explicit LightSubpass(InjectionContext &context);
        LightSubpass(const LightSubpass &) = delete;
        LightSubpass(LightSubpass &&) = delete;

        LightSubpass &operator=(const LightSubpass &) = delete;
        LightSubpass &operator=(LightSubpass &&) = delete;

        void prepare(const Core::SharedCommandBuffer &commandBuffer) override;

        /// \brief Record draw commands
        void draw(const Core::SharedCommandBuffer &commandBuffer) override;

        std::type_index getType() const override;

    private:
        SharedLightRenderer renderer;
    };
}


#endif //DEFERRED_LIGHTSUBPASS_H
